fs = 48e3;
t = 0:1/fs:1-1/fs;
A = 50;
x = A*cos(2*pi*1000*t);
y = awgn(x,40,'measured');
snr(y)