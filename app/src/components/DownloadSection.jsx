import { motion } from "framer-motion";
import GlassContainer from "./GlassContainer";
import { Download as DownloadIcon, Monitor, Apple, Terminal } from "lucide-react";

const spring = { type: "spring", stiffness: 100, damping: 20 };

const platforms = [
  { icon: Monitor, label: "Windows", version: "v2.0.1", size: "24 MB" },
  { icon: Apple, label: "macOS", version: "v2.0.1", size: "22 MB" },
  { icon: Terminal, label: "Linux", version: "v2.0.1", size: "20 MB" },
];

export default function DownloadSection() {
  return (
    <section id="download" className="relative px-6 py-24 max-w-6xl mx-auto">
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        whileInView={{ opacity: 1, y: 0 }}
        viewport={{ once: true }}
        transition={spring}
        className="text-center mb-16"
      >
        <div className="inline-flex items-center gap-2 px-4 py-1.5 rounded-full bg-white/[0.05] border border-white/[0.08] mb-6">
          <DownloadIcon className="w-4 h-4 text-white/60" strokeWidth={1.5} />
          <span className="text-xs font-medium text-white/50 uppercase tracking-widest">
            Download
          </span>
        </div>
        <h2 className="text-3xl sm:text-4xl font-bold tracking-tight text-white/90">
          Get Mozority
        </h2>
        <p className="mt-4 text-white/40 max-w-lg mx-auto text-sm leading-relaxed">
          Available for all major platforms. One click, zero configuration.
        </p>
      </motion.div>

      <div className="grid grid-cols-1 sm:grid-cols-3 gap-5 max-w-3xl mx-auto">
        {platforms.map((p, i) => (
          <motion.div
            key={p.label}
            initial={{ opacity: 0, y: 30 }}
            whileInView={{ opacity: 1, y: 0 }}
            viewport={{ once: true, amount: 0.2 }}
            transition={{ ...spring, delay: i * 0.12 }}
            whileHover={{ scale: 1.04, y: -4 }}
          >
            <GlassContainer
              animate={false}
              variant="dark"
              className="p-6 flex flex-col items-center text-center gap-4 cursor-pointer group"
            >
              <div className="w-14 h-14 rounded-2xl bg-white/[0.06] border border-white/[0.08] flex items-center justify-center group-hover:bg-white/[0.1] transition-colors">
                <p.icon className="w-7 h-7 text-white/70" strokeWidth={1.5} />
              </div>
              <div>
                <h3 className="text-lg font-semibold text-white/90">{p.label}</h3>
                <p className="text-xs text-white/30 mt-1">{p.version} · {p.size}</p>
              </div>
              {/* Download button with sheen */}
              <div className="relative mt-2 w-full py-2.5 rounded-xl bg-white/[0.08] border border-white/[0.1] text-sm font-medium text-white/80 overflow-hidden group/btn hover:bg-white/[0.12] transition-colors">
                <span className="absolute inset-0 -translate-x-full group-hover/btn:translate-x-full transition-transform duration-700 ease-in-out bg-gradient-to-r from-transparent via-white/15 to-transparent" />
                <span className="relative z-10 flex items-center justify-center gap-1.5">
                  <DownloadIcon className="w-4 h-4" strokeWidth={1.5} />
                  Download
                </span>
              </div>
            </GlassContainer>
          </motion.div>
        ))}
      </div>
    </section>
  );
}
