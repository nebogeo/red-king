% Run this code in matlab to call the function and plot the output

tic;
system('./range_spedup_final')
toc;

X=load('range_spedup_final.txt');
L=length(X(1,:))/2;

umin = 0;
umax = 10;
vmin = 0;
vmax = 10;

H=X(:,1:L); 
P=X(:,(L+1):end); 

H=H./repmat(sum(H,2),[1,L]);
P=P./repmat(sum(P,2),[1,L]);


H0=log10(H);
H0(H0<-5)=-5;
H0=(H0+5)/5;

P0=log10(P);
P0(P0<-5)=-5;
P0=(P0+5)/5;

% These figures show how the host and parasite traits coevolve
% Could plot these while the solver is running so that the user
% could see the trees 'growing' and branching/cycling etc. Could
% sonify this output: e.g. pitch = trait values, multiple traits
% give harmonics, cycling gives fluctuating pitch.

figure(100)
clf
subplot(1,2,1)
image(64*H0);
set(gca,'ydir','normal')
xlabel('Host range, u')
ylabel('Evolutionary time')
set(gca,'xtick',0.5+[0,L/2,L],'xticklabel',[umin,(umax-umin)/2,umax])

subplot(1,2,2)
image(64*P0);
set(gca,'ydir','normal')
xlabel('Parasite range, v')
ylabel('Evolutionary time')
set(gca,'xtick',0.5+[0,L/2,L],'xticklabel',[vmin,(vmax-vmin)/2,vmax])
map=colormap('gray');colormap(flipud(map))


% Analyse output - this is pretty straightforward. 
% User could do this via sonification.
[PKS_H,LOCS_H]= findpeaks([0,H(end,:),0]);
[PKS_P,LOCS_P]= findpeaks([0,P(end,:),0]);

num_hosts = length(LOCS_H);
num_parasites = length(LOCS_P);

% If run for longer, can also look at variance in traits over 
% time to detect cycling