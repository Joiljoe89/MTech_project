roi = [5 280 640 340];
% Maximum number of objects to track
maxNumObj = 20;
% Number of frames that an object must remain stationary before an alarm is
% raised
alarmCount = 5;
% Maximum number of frames that an abandoned object can be hidden before it
% is no longer tracked
maxConsecutiveMiss = 2;
areaChangeFraction = 13;     % Maximum allowable change in object area in percent
centroidChangeFraction = 18; % Maximum allowable change in object centroid in percent
% Minimum ratio between the number of frames in which an object is detected
% and the total number of frames, for that object to be tracked.
minPersistenceRatio = 0.7;
% Offsets for drawing bounding boxes in original input video
PtsOffset = int32(repmat([roi(1), roi(2), 0, 0],[maxNumObj 1]));

hVideoSrc = imaq.VideoDevice('winvideo', 1, 'YUY2_640x480');
%hVideoSrc = imaq.VideoDevice('winvideo', 2, 'UYVY_720x480');
set(hVideoSrc,'ReturnedColorSpace','rgb');
hColorConv = vision.ColorSpaceConverter('Conversion', 'RGB to YCbCr');
hAutothreshold = vision.Autothresholder('ThresholdScaleFactor', 1.3);
hClosing = vision.MorphologicalClose('Neighborhood', strel('square',5));

hBlob = vision.BlobAnalysis('MaximumCount', maxNumObj, 'ExcludeBorderBlobs', true);
hBlob.MinimumBlobArea = 100;
hBlob.MaximumBlobArea = 5000;

pos = [10 300 roi(3)+25 roi(4)+25];
hAbandonedObjects = vision.VideoPlayer('Name', 'Abandoned Objects', 'Position', pos);
pos(1) = 46+roi(3); % move the next viewer to the right
hAllObjects = vision.VideoPlayer('Name', 'All Objects', 'Position', pos);
pos = [80+2*roi(3) 300 roi(3)-roi(1)+25 roi(4)-roi(2)+25];
hThresholdDisplay = vision.VideoPlayer('Name', 'Threshold', 'Position', pos);
n=0;
firsttime = true;
while (n<300)
    Im = step(hVideoSrc);

    % Select the region of interest from the original video
    OutIm = Im(roi(2):end, roi(1):end, :);

    YCbCr = step(hColorConv, OutIm);
    CbCr  = complex(YCbCr(:,:,2), YCbCr(:,:,3));

    % Store the first video frame as the background
    if firsttime
        firsttime = false;
        BkgY      = YCbCr(:,:,1);
        BkgCbCr   = CbCr;
    end
    SegY    = step(hAutothreshold, abs(YCbCr(:,:,1)-BkgY));
    SegCbCr = abs(CbCr-BkgCbCr) == 0 ;

    % Fill in small gaps in the detected objects
    Segmented = step(hClosing, SegY | SegCbCr);

    % Perform blob analysis
    [Area, Centroid, BBox] = step(hBlob, Segmented);

    % Call the helper function that tracks the identified objects and
    % returns the bounding boxes and the number of the abandoned objects.
    [OutCount, OutBBox] = videoobjtracker(Area, Centroid, BBox, maxNumObj, ...
       areaChangeFraction, centroidChangeFraction, maxConsecutiveMiss, ...
       minPersistenceRatio, alarmCount);
   
    % Display the abandoned object detection results
    Imr = insertShape(Im,'FilledRectangle',OutBBox+PtsOffset,...
        'Color','red','Opacity',0.5);
    % insert number of abandoned objects in the frame
    Imr = insertText(Imr, [128 356], OutCount);
    step(hAbandonedObjects, Imr);

    BlobCount = size(BBox,1);

    BBoxOffset = BBox + int32(repmat([roi(1) roi(2) 0  0],[BlobCount 1]));
    Imr = insertShape(Im,'Rectangle',BBoxOffset,'Color','green');

    % Display all the detected objects

    % insert number of all objects in the frame
    Imr = insertText(Imr, [64 356], OutCount);
    Imr = insertShape(Imr,'Rectangle',roi);
    %Imr = step(hDrawBBox, Imr, roi);
    step(hAllObjects, Imr);

    % Display the segmented video
    SegBBox = PtsOffset;
    SegBBox(1:BlobCount,:) = BBox;
    SegIm = insertShape(double(repmat(Segmented,[1 1 3])),'Rectangle', SegBBox,'Color','green');
    %SegIm = step(hDrawRectangles3, repmat(Segmented,[1 1 3]), SegBBox);
    step(hThresholdDisplay, SegIm);
    n=n+1;
end

release(hVideoSrc);
delete(hAbandonedObjects);
delete(hAllObjects);
delete(hThresholdDisplay);
clear;