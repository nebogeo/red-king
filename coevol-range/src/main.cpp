#include <iostream>
#include "jellyfish/audio.h"
#include "range_spedup_final.h"
#include "coevol_stepper.h"
#include "additive_synth.h"


using namespace std;


audio_device *audio = NULL;

#define BANDS 100
additive_synth synth(BANDS);
additive_synth synth2(BANDS);
int frame=0;

void run_audio(void* c, unsigned int frames) {
    audio->left_out.zero();
    coevol_stepper *s = (coevol_stepper *)c;
    s->step();

    for (u32 i=0; i<N; i++) {
        synth2.set_level(i%BANDS,s->x0[i]);
        synth.set_level(i%BANDS,s->y[i]);
        //synth.set_level(i,0.5+0.5*sin((i+(frame*10))/20.0));
    }
    frame++;

    synth.render(audio->left_out);
    synth2.render(audio->right_out);

    //s->process(frames,a->left_out.get_non_const_buffer());
//    sleep(1);


}


int main() {

    double u[N], v[N], a[N], beta[N], E[N][N], **xout;
    int i, j;
    char filename[100];

    /* Initialise discretised trait values */
	for (i=0; i<N; i++) {
		u[i]=UMIN+(UMAX-UMIN)*i/(N-1); /* Host */
		v[i]=VMIN+(VMAX-VMIN)*i/(N-1); /* Parasite */
	}

    /**********************************************************************
     * This section is where the trade-offs and host-parasite interactions
     * are defined (i.e. where the user would make changes via a GUI)
     *********************************************************************/

    /* Define cost functions (trade-offs) */
	for (i=0; i<N; i++) {
		a[i]=AMAX-(AMAX-AMIN)*(1-(u[i]-UMAX)/(UMIN-UMAX))/(1+A_P*(u[i]-UMAX)/(UMIN-UMAX)); /* Host trade-off */
		beta[i]=BEMAXTIME-(BEMAXTIME-BETMIN)*(1-(v[i]-VMAX)/(VMIN-VMAX))/(1+BETA_P*(v[i]-VMAX)/(VMIN-VMAX)); /* Parasite trade-off */
	}

    /* Define host-parasite interaction matrix */
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            E[i][j] = beta[j]*(1-1/(1+exp(-2*(u[i]-v[j]))));
        }
    }

    /**********************************************************************
     * End of user-defined section
     *********************************************************************/

    /* Call adaptive dynamics routine (main solver) */
    xout = array_maker(2*N+2,NEVOL);

    coevol_stepper stepper(xout, &u[0], &v[0], (double **)&E[0][0], &a[0], &beta[0]);

    audio = new audio_device("evolution",44100,2048);
	audio->m_client.set_callback(run_audio, &stepper);

    synth.set_freq(0.01);
    synth2.set_freq(0.01);

    for (;;) { sleep(1); }

    return 1;
}
