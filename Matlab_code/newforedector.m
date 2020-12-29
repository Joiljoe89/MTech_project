foregroundDetector = vision.ForegroundDetector('NumGaussians', 3, ...
    'NumTrainingFrames', 50);
roi = [275 5 300 380];
%videoReader = imaq.VideoDevice('winvideo', 1, 'YUY2_640x480');
videoReader = imaq.VideoDevice('winvideo', 2, 'UYVY_720x576');
set(videoReader,'ReturnedColorSpace','rgb');
videoPlayer1 = vision.VideoPlayer('Name', 'Detected');
videoPlayer1.Position(3:4) = [850,600];  % window size: [width, height]
videoPlayer2 = vision.VideoPlayer('Name', 'ROI');
videoPlayer2.Position(3:4) = [650,400]; 
se = strel('square', 3);
blobAnalysis = vision.BlobAnalysis('BoundingBoxOutputPort', true, ...
    'AreaOutputPort', false, 'CentroidOutputPort', false, ...
    'MinimumBlobArea', 1250,'MaximumBlobArea', 15000 );
frame1=0;
c=0;
while(c<400)
    %frame1=getdata(videoReader, 1, 'single');
    frame1 = step(videoReader); 
    foreground = step(foregroundDetector, frame1);
    filteredForeground = imopen(foreground, se);
    bbox = step(blobAnalysis, filteredForeground);
    result = insertShape(frame1, 'Rectangle', bbox, 'Color', 'green');
    % Display the number of cars found in the video frame
    numCars = size(bbox, 1);
    result = insertText(result, [10 10], numCars, 'BoxOpacity', 1, ...
        'FontSize', 14);
    OutIm =  result(roi(1):end, roi(2):end, :);
    step(videoPlayer1, result); 
    step(videoPlayer2, OutIm); 
    c=c+1;
end
release(videoReader);
delete(videoPlayer1); 
delete(videoPlayer2);