num=[6.122];
den=[1 0 -60];
numlead=-7;
denlead=-20;
[Z,P,K]=tf2zp(num,den);
Za=[Z;numlead];
Pa=[P;denlead];
[num2,den2]=zp2tf(Za,Pa,K);
sys=tf(num2,den2);
rlocus(sys);
figure
KK=41.2057;
sys2=zpk(Za,Pa,KK*K);
sysc=sys2/(1+sys2);
t=0:0.01:3;
step(sysc,t);