#include "additive_synth.h"

using namespace spiralcore;

additive_synth::additive_synth(u32 num_oscs) {
    m_num_oscs = num_oscs;
    m_oscs = new float[num_oscs];
    m_level = new float[num_oscs];
}

void additive_synth::set_level(u32 n, float l) {
    m_level[n]=l;
}

void additive_synth::render(sample &out) {
    for (u32 i=0; i<out.get_length(); ++i) {
        out[i]=0;
        for (u32 b=0; b<m_num_oscs; ++b) {
            out[i]+=0.1*sin(m_oscs[b])*m_level[b];
            m_oscs[b]+=m_freq*(b+1);
        }
    }
}
