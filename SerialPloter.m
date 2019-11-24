%MATLAB Code for Serial Communication between Arduino and MATLAB and plot
a1 = 306.2;
b1 = 2.409;
b2 = -0.000801;
b3 = -0.0000063;
b4 = 0.000000009;
b5 = 0.00000000000204;
%serial port data
run = true;
x=serial('COM5','BAUD', 115200); %change COM4 to the active com port 
x.ReadAsyncMode = 'manual';
x.InputBufferSize = 4608;

fopen(x);
%--reads data out of the serial port and devides it in to sections and averages--%
    data = fscanf(x,char, 4608); 
    c1 = regexp(data, ',', 'split');
    backGroundData = str2double(c1);
    backGroundData = interp1([1,1024],[1,100],backGroundData);
        cutData1 = backGroundData(1:288);
        cutData2 = backGroundData(289:577);
        cutData2 = cutData2(1:288);
        cutData3 = backGroundData(578:865);
        cutData4 = backGroundData(866:1153);
    averageData1 = (cutData2 + cutData3 + cutData4)/3;
   
    %--sets up the X axis for ploting --% 
   % waveLength = [325.46:0.78:550.1];
   pix = [1:288];
   waveLength = a1 + b1*pix + b2*pix.^2 + b3*(pix).^3 + b4*pix.^4 + b5*pix.^5;
   XAxis = waveLength(1:288);
%--340:1.7708:850--%
 
%--plots the data without a sample--%
   figure(1);
     plot(XAxis,averageData1);
     grid;
     title('Background values Plot');
     ylabel('intensity');
     xlabel('wavelength');

   
fclose(x);
delete(x);
%--waights hear till a button is presed then moves on --%
w = waitforbuttonpress;
%--reopens the serial com port --%
x=serial('COM5','BAUD', 115200); %change COM4 to the active com port 
x.ReadAsyncMode = 'manual';
x.InputBufferSize = 4608;

fopen(x);
%-- reads a new set of data with a sample present --%
    data = fscanf(x,char, 4608); 
    c2 = regexp(data, ',', 'split');
    SampleData = str2double(c2);   
    SampleData = interp1([1,1024],[1,100],SampleData);
        sampleCutData1 = SampleData(1:288);
        sampleCutData2 = SampleData(289:577);
        sampleCutData2 = sampleCutData2(1:288);
        sampleCutData3 = SampleData(578:865);
        sampleCutData4 = SampleData(866:1153);
    averageData2 = (sampleCutData2 + sampleCutData3 + sampleCutData4)/3;
  %--plots the samples data--% 
  figure(2);
   plot(XAxis,averageData2);
   grid;
   title('Sample values Plot');
   ylabel('intencity');
   xlabel('wave Length');
   
fclose(x);
delete(x);
%--calculates the diference between the 2 resolts--%  
difData = averageData2 - averageData1;
%--plots the diference --%
figure(3)
plot (XAxis, difData, 'b--o');
grid 
title('change in light');
    ylabel('intencity');
    xlabel('wave Length')
  