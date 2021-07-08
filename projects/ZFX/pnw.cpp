#include <zeno/zeno.h>
#include <zeno/zfx.h>
#include <zeno/StringObject.h>
#include <zeno/PrimitiveObject.h>
#include <zeno/NumericObject.h>
#include <zeno/ListObject.h>
#include <cassert>

struct Buffer {
    float *base = nullptr;
    size_t count = 0;
    size_t stride = 0;
    int which = 0;
};

template <class F>
static void vectors_neighbor_wrangle
    ( zfx::Program const *prog
    , std::vector<zeno::vec3f> const &pos
    , std::vector<Buffer> const &chs
    , std::vector<float> const &pars
    , size_t size1, size_t size2
    , F const &get_neighbor
    ) {
    if (chs.size() == 0)
        return;
    #pragma omp parallel for
    for (int i1 = 0; i1 < size1; i1++) {
        zfx::Context ctx;
        for (int j = 0; j < pars.size(); j++) {
            ctx.regtable[j] = pars[j];
        }
        for (int j = 0; j < chs.size(); j++) {
            if (chs[j].which == 0)
                ctx.memtable[j] = chs[j].base + chs[j].stride * i1;
        }
        for (int i2: get_neighbor(pos[i1])) {
            for (int j = 0; j < chs.size(); j++) {
                if (chs[j].which == 1)
                    ctx.memtable[j] = chs[j].base + chs[j].stride * i2;
            }
            prog->execute(&ctx);
        }
    }
}

struct ParticlesNeighborWrangle : zeno::INode {
    virtual void apply() override {
        auto prim1 = get_input<zeno::PrimitiveObject>("prim1");
        auto prim2 = get_input<zeno::PrimitiveObject>("prim2");
        auto radius = get_input<zeno::NumericObject>("radius")->get<float>();

        auto code = get_input<zeno::StringObject>("zfxCode")->get();
        std::ostringstream oss;
        for (auto const &[key, attr]: prim1->m_attrs) {
            oss << "define ";
            std::visit([&oss] (auto const &v) {
                using T = std::decay_t<decltype(v[0])>;
                if constexpr (std::is_same_v<T, zeno::vec3f>) oss << "f3";
                else if constexpr (std::is_same_v<T, float>) oss << "f1";
                else oss << "unknown";
            }, attr);
            oss << " @" << key << '\n';
        }
        for (auto const &[key, attr]: prim2->m_attrs) {
            oss << "define ";
            std::visit([&oss] (auto const &v) {
                using T = std::decay_t<decltype(v[0])>;
                if constexpr (std::is_same_v<T, zeno::vec3f>) oss << "f3";
                else if constexpr (std::is_same_v<T, float>) oss << "f1";
                else oss << "unknown";
            }, attr);
            oss << " @" << key << ":j" << '\n';
        }

        auto params = get_input<zeno::ListObject>("params");
        std::vector<float> pars;
        std::vector<std::string> parnames;
        for (int i = 0; i < params->arr.size(); i++) {
            auto const &obj = params->arr[i];
            std::ostringstream keyss; keyss << "arg" << i;
            auto key = keyss.str();
            auto par = dynamic_cast<zeno::NumericObject *>(obj.get());
            oss << "define ";
            std::visit([&] (auto const &v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, zeno::vec3f>) {
                    oss << "f3";
                    pars.push_back(v[0]);
                    pars.push_back(v[1]);
                    pars.push_back(v[2]);
                    parnames.push_back(key + ".0");
                    parnames.push_back(key + ".1");
                    parnames.push_back(key + ".2");
                } else if constexpr (std::is_same_v<T, float>) {
                    oss << "f1";
                    pars.push_back(v);
                    parnames.push_back(key + ".0");
                } else oss << "unknown";
            }, par->value);
            oss << " " << key << '\n';
        }
        for (auto const &par: parnames) {
            oss << "parname " << par << '\n';
        }

        code = oss.str() + code;
        auto prog = zfx::compile_program(code);

        std::vector<Buffer> chs(prog->channels.size());
        for (int i = 0; i < chs.size(); i++) {
            auto channe = zfx::split_str(prog->channels[i], '.');
            auto chan = zfx::split_str(channe[0], ':');
            if (chan.size() == 1) {
                chan.push_back("i");
            }
            assert(chan.size() == 2);
            int dimid = 0;
            std::stringstream(channe[1]) >> dimid;
            Buffer iob;
            auto const &attr = prim1->attr(chan[0]);
            std::visit([&] (auto const &arr) {
                iob.base = (float *)arr.data() + dimid;
                iob.count = arr.size();
                iob.stride = sizeof(arr[0]) / sizeof(float);
            }, attr);
            iob.which = chan[1][0] - 'i';
            chs[i] = iob;
        }
        auto const &p1pos = prim1->attr<zeno::vec3f>("pos");
        auto const &p2pos = prim2->attr<zeno::vec3f>("pos");
        vectors_neighbor_wrangle(prog, p1pos,
            chs, pars, prim1->size(), prim2->size(), [&](zeno::vec3f const &p) {
                std::vector<int> res;
                for (int i = 0; i < p2pos.size(); i++) {
                    if (zeno::length(p2pos[i] - p) < radius) {
                        res.push_back(i);
                    }
                }
                return res;
            });

        set_output("prim1", std::move(prim1));
    }
};

ZENDEFNODE(ParticlesNeighborWrangle, {
    {"prim1", "prim2", "radius", "zfxCode", "params"},
    {"prim1"},
    {},
    {"zenofx"},
});
