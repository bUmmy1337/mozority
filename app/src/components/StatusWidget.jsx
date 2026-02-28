import { motion } from "framer-motion";
import GlassContainer from "./GlassContainer";

const rows = [
  { label: "System", value: "Online", dot: "bg-emerald-400" },
  { label: "Version", value: "2.0", dot: "bg-white/40" },
  { label: "Security", value: "Undetected", dot: "bg-emerald-400" },
];

export default function StatusWidget() {
  return (
    <section id="status" className="relative px-6 py-24 max-w-6xl mx-auto flex justify-center">
      <GlassContainer variant="dark" className="w-full max-w-sm p-6">
        <h3 className="text-xs font-semibold uppercase tracking-widest text-white/30 mb-5">
          System Status
        </h3>
        <div className="flex flex-col gap-4">
          {rows.map((r, i) => (
            <motion.div
              key={r.label}
              initial={{ opacity: 0, x: -10 }}
              whileInView={{ opacity: 1, x: 0 }}
              viewport={{ once: true }}
              transition={{
                type: "spring",
                stiffness: 100,
                damping: 20,
                delay: 0.15 * i,
              }}
              className="flex items-center justify-between"
            >
              <div className="flex items-center gap-2.5">
                <span className={`w-2 h-2 rounded-full ${r.dot}`} />
                {r.dot.includes("emerald") && (
                  <span className={`absolute w-2 h-2 rounded-full ${r.dot} animate-ping opacity-75`} />
                )}
                <span className="text-sm text-white/50">{r.label}</span>
              </div>
              <span className="text-sm font-medium text-white/80">{r.value}</span>
            </motion.div>
          ))}
        </div>
        {/* Decorative line */}
        <div className="mt-6 h-px bg-gradient-to-r from-transparent via-white/10 to-transparent" />
        <p className="mt-4 text-[11px] text-white/20 text-center">
          Last checked: just now
        </p>
      </GlassContainer>
    </section>
  );
}
