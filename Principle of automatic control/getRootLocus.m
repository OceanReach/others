coscossd=zeta;
sita=acos(coscossd);
sincossd=sin(sita);
wn=4/(ts*zeta)+wnoffset;
sd1=wn*(-coscossd+j*sincossd);
sd2=wn*(-coscossd-j*sincossd);
GSd=-(atan(wn*sincossd/(wn*coscossd-7.746))+atan(wn*sincossd/(wn*coscossd+7.746)));
fi=pi-GSd;
gama=(GSd-sita)/2;

zc=wn*sin(gama)/sin(pi-sita-gama);
zp=wn*sin(gama+fi)/sin(pi-sita-gama-fi);
a=(sd1+zc)/(sd1+zp);
b=6.122/(sd1^2-60);
c=a*b;
K=1/abs(c);
