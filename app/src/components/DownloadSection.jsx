import { motion } from "framer-motion";
import GlassContainer from "./GlassContainer";
import { Download as DownloadIcon, Monitor } from "lucide-react";

const spring = { type: "spring", stiffness: 100, damping: 20 };

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
          Windows only. One click, zero configuration.
        </p>
      </motion.div>

      <div className="flex justify-center max-w-sm mx-auto">
        <motion.div
          initial={{ opacity: 0, y: 30 }}
          whileInView={{ opacity: 1, y: 0 }}
          viewport={{ once: true, amount: 0.2 }}
          transition={spring}
          whileHover={{ scale: 1.04, y: -4 }}
          className="w-full"
        >
          <GlassContainer
            animate={false}
            variant="dark"
            className="p-8 flex flex-col items-center text-center gap-5 cursor-pointer group"
          >
            <div className="w-16 h-16 rounded-2xl bg-white/[0.06] border border-white/[0.08] flex items-center justify-center group-hover:bg-white/[0.1] transition-colors">
              <Monitor className="w-8 h-8 text-white/70" strokeWidth={1.5} />
            </div>
            <div>
              <h3 className="text-xl font-semibold text-white/90">Windows</h3>
              <p className="text-xs text-white/30 mt-1">v2.0.1 · 24 MB</p>
            </div>
            {/* Download button with sheen */}
            <div className="relative mt-1 w-full py-3 rounded-xl bg-white/[0.08] border border-white/[0.1] text-sm font-medium text-white/80 overflow-hidden group/btn hover:bg-white/[0.12] transition-colors">
              <span className="absolute inset-0 -translate-x-full group-hover/btn:translate-x-full transition-transform duration-700 ease-in-out bg-gradient-to-r from-transparent via-white/15 to-transparent" />
              <span className="relative z-10 flex items-center justify-center gap-2">
                <DownloadIcon className="w-4 h-4" strokeWidth={1.5} />
                Download for Windows
              </span>
            </div>
          </GlassContainer>
        </motion.div>
      </div>
    </section>
  );
}
