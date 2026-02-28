import { motion } from "framer-motion";
import GlassContainer from "./GlassContainer";
import { Shield, Fingerprint, EyeOff, ServerCrash, Lock, ShieldCheck } from "lucide-react";

const spring = { type: "spring", stiffness: 100, damping: 20 };

const items = [
  {
    icon: Fingerprint,
    title: "Zero Fingerprint",
    desc: "Leaves no traceable artifacts. Every session is ephemeral by design.",
  },
  {
    icon: EyeOff,
    title: "Anti-Detection",
    desc: "Bypasses all known scanning heuristics with polymorphic execution layers.",
  },
  {
    icon: ServerCrash,
    title: "Crash Resistant",
    desc: "Graceful degradation and auto-recovery ensure uptime even under stress.",
  },
  {
    icon: Lock,
    title: "AES-256 Streams",
    desc: "All data channels are end-to-end encrypted with rotating session keys.",
  },
];

export default function Security() {
  return (
    <section id="security" className="relative px-6 py-24 max-w-6xl mx-auto">
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        whileInView={{ opacity: 1, y: 0 }}
        viewport={{ once: true }}
        transition={spring}
        className="text-center mb-16"
      >
        <div className="inline-flex items-center gap-2 px-4 py-1.5 rounded-full bg-white/[0.05] border border-white/[0.08] mb-6">
          <ShieldCheck className="w-4 h-4 text-emerald-400/80" strokeWidth={1.5} />
          <span className="text-xs font-medium text-white/50 uppercase tracking-widest">
            Security First
          </span>
        </div>
        <h2 className="text-3xl sm:text-4xl font-bold tracking-tight text-white/90">
          Unbreakable by Design
        </h2>
        <p className="mt-4 text-white/40 max-w-lg mx-auto text-sm leading-relaxed">
          Every layer of Mozority is engineered with security as the foundation — not an afterthought.
        </p>
      </motion.div>

      <div className="grid grid-cols-1 sm:grid-cols-2 gap-5">
        {items.map((item, i) => (
          <motion.div
            key={item.title}
            initial={{ opacity: 0, y: 30 }}
            whileInView={{ opacity: 1, y: 0 }}
            viewport={{ once: true, amount: 0.2 }}
            transition={{ ...spring, delay: i * 0.1 }}
            whileHover={{ scale: 1.02 }}
          >
            <GlassContainer animate={false} className="p-6 h-full flex gap-4">
              <div className="w-10 h-10 shrink-0 rounded-xl bg-white/[0.06] border border-white/[0.08] flex items-center justify-center">
                <item.icon className="w-5 h-5 text-white/70" strokeWidth={1.5} />
              </div>
              <div>
                <h3 className="text-base font-semibold text-white/90 mb-1">{item.title}</h3>
                <p className="text-sm leading-relaxed text-white/40">{item.desc}</p>
              </div>
            </GlassContainer>
          </motion.div>
        ))}
      </div>
    </section>
  );
}
