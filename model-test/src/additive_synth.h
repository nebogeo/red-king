#include <jellyfish/core/types.h>
#include <jellyfish/fluxa/sample.h>

namespace spiralcore {

class additive_synth {
public:
    additive_synth(u32 num_oscs);
    void set_freq(float f) { m_freq=f; }
    void set_level(u32 n, float l);
    void render(sample &out);


private:

    float *m_level;
    float *m_level_actual;
    float *m_oscs;
    float m_freq;
    u32 m_num_oscs;
};


}
