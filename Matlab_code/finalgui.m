function varargout = finalgui(varargin)
% FINALGUI MATLAB code for finalgui.fig
%      FINALGUI, by itself, creates a new FINALGUI or raises the existing
%      singleton*.
%
%      H = FINALGUI returns the handle to a new FINALGUI or the handle to
%      the existing singleton*.
%
%      FINALGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in FINALGUI.M with the given input arguments.
%
%      FINALGUI('Property','Value',...) creates a new FINALGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before finalgui_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to finalgui_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help finalgui

% Last Modified by GUIDE v2.5 13-Apr-2014 19:57:49

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @finalgui_OpeningFcn, ...
                   'gui_OutputFcn',  @finalgui_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT
% create arduino object and connect to board

% --- Executes just before finalgui is made visible.
function finalgui_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to finalgui (see VARARGIN)

% Choose default command line output for finalgui
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes finalgui wait for user response (see UIRESUME)
% uiwait(handles.figure1);

% --- Outputs from this function are returned to the command line.
function varargout = finalgui_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbuttonscan.
function pushbuttonscan_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonscan (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%function serialcommnradar(varargin)
 gid  =  figure;
   style  =  [0.5,1,0.9];
   styleS  =  'g';
   whitebg(gid,'k');
    a = 300;
    rMax  =  ceil(max(mod(a,0)));
  circle([0,0],rMax,360,style);
  hold on;
  circle([0,0],rMax * (2/3),360,style);
  circle([0,0],rMax * (1/3),360,style);
  radialaxis(0,rMax,style);
  radialaxis(10,rMax,style);
  radialaxis(20,rMax,style);
  radialaxis(30,rMax,style);
  radialaxis(40,rMax,style);
  radialaxis(50,rMax,style);
  radialaxis(330,rMax,style);
  radialaxis(340,rMax,style);
  radialaxis(350,rMax,style);
  radialaxis(45,rMax,style);
  radialaxis(90,rMax,style);
  radialaxis(135,rMax,style);
  radialaxis(180,rMax,style);
  radialaxis(225,rMax,style);
  radialaxis(270,rMax,style);
  radialaxis(315,rMax,style);
  
  s=serial('COM29','BaudRate',9600);
  s.ReadAsyncMode = 'continuous';
  fopen(s);
  i=1;
  direction = 120;
  fprintf(s,'%s','D');
 while (i<=9)
  C=fscanf(s);
  fprintf(s,'%s','D');
  e = ',.';
  [a,b] = regexp(C, e,'match','split');
  n = strjoin(b);
  disp(n);
  m = n(1:3);
  speed = str2double(m);
  disp(speed);
  
  %whos

  cosMask  =  unique([find(direction <= 90),find(direction  >=  270)]);
  sinMask  =  find(direction  <  270  &  direction  >  90);
  
  u(cosMask)  =  speed(cosMask)  .*  cosd(direction(cosMask));
  v(cosMask)  =  speed(cosMask)  .*  sind(direction(cosMask));
  
  u(sinMask)  =  speed(sinMask)  .*  cosd(direction(sinMask));
  v(sinMask)  =  speed(sinMask)  .*  sind(direction(sinMask));
  
  direction = direction-10; 
  disp(u);
  disp(v);
  scatter(u,v,[],styleS);
  i=i+1;
  %hold off;
end
fclose(s)
   
  function H=circle(center,radius,NOP,style)
    THETA=linspace(0,2*pi,NOP);
    RHO=ones(1,NOP)*radius;
    [X,Y] = pol2cart(THETA,RHO);
    X=X+center(1);
    Y=Y+center(2);
    H=plot(X,Y);
    set(H,'Color',style);
    axis square;

%--------------------------------------------------------------------------
function H  =  radialaxis(angle,maxx,plotColour)
    x(1)  =  0;
    y(1)  =  0;
    if(angle  >=  270  &&  angle  <=  90)
      x(2)  =  maxx * cosd(angle);
      y(2)  =  maxx * sind(angle);
    else
      y(2)  =  maxx * cosd(angle);
      x(2)  =  maxx * sind(angle);
    end
    H  =  plot(x,y);
    set(H,'Color',plotColour);


% --- Executes on button press in STOP.
function STOP_Callback(hObject, eventdata, handles)
% hObject    handle to STOP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
 s=serial('COM29','BaudRate',9600);
 s.ReadAsyncMode = 'continuous';
 fopen(s);
 fprintf(s,'%s','S');
 fclose(s)

% --- Executes on button press in FORWARD.
function FORWARD_Callback(hObject, eventdata, handles)
% hObject    handle to FORWARD (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
 s=serial('COM29','BaudRate',9600);
 s.ReadAsyncMode = 'continuous';
 fopen(s);
 fprintf(s,'%s','F');
 fclose(s)

% --- Executes on button press in REVERSE.
function REVERSE_Callback(hObject, eventdata, handles)
% hObject    handle to REVERSE (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
 s=serial('COM29','BaudRate',9600);
 s.ReadAsyncMode = 'continuous';
 fopen(s);
 fprintf(s,'%s','B');
 fclose(s)


% --- Executes on button press in LEFT.
function LEFT_Callback(hObject, eventdata, handles)
% hObject    handle to LEFT (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
 s=serial('COM29','BaudRate',9600);
 s.ReadAsyncMode = 'continuous';
 fopen(s);
 fprintf(s,'%s','L');
 fclose(s)

% --- Executes on button press in RIGHT.
function RIGHT_Callback(hObject, eventdata, handles)
% hObject    handle to RIGHT (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
 s=serial('COM29','BaudRate',9600);
 s.ReadAsyncMode = 'continuous';
 fopen(s);
 fprintf(s,'%s','R');
 fclose(s)


% --- Executes on button press in video.
function video_Callback(hObject, eventdata, handles)
% hObject    handle to video (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
axes(handles.axes3);
vidObj1 = videoinput('winvideo', 1, 'YUY2_640x480');
%vidObj1 = videoinput('winvideo', 2, 'UYVY_720x480');
videoRes = get(vidObj1, 'VideoResolution');
numberOfBands = get(vidObj1, 'NumberOfBands');
handleToImage = image( zeros([videoRes(2), videoRes(1), numberOfBands], 'uint16') );
preview(vidObj1, handleToImage);


% --- Executes on button press in pushbuttonSCANSTART.
function pushbuttonSCANSTART_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonSCANSTART (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
s=serial('COM29','BaudRate',9600);
 s.ReadAsyncMode = 'continuous';
 fopen(s);
 fprintf(s,'%s','A');
 fclose(s)


% --- Executes on button press in pushbuttonmanual.
function pushbuttonmanual_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonmanual (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbuttonauto.
function pushbuttonauto_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonauto (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
