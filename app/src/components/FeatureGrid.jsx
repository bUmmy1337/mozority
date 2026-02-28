import { motion } from "framer-motion";
import GlassContainer from "./GlassContainer";
import { Crosshair, Eye, Paintbrush, Moon, Zap, Move } from "lucide-react";

const features = [
  {
    icon: Paintbrush,
    title: "Skin Changer",
    desc: "Complete inventory replacement — weapons, knives, gloves, agents. Customization of wear, pattern, StatTrak.",
  },
  {
    icon: Eye,
    title: "ESP",
    desc: "Boxes, health bars, player names, real-time previews.",
  },
  {
    icon: Crosshair,
    title: "Aimbot",
    desc: "Configurable aimbot with smoothness. Unsafe feature — must be enabled in settings.",
  },
  {
    icon: Moon,
    title: "Nightmode",
    desc: "Recoloring world materials (night mode).",
  },
  {
    icon: Zap,
    title: "Chams",
    desc: "Highlighting player models through walls.",
  },
  {
    icon: Move,
    title: "Movement",
    desc: "BunnyHop and other assists. Also requires enabling unsafe features.",
  },
];

const cardVariants = {
  hidden: { opacity: 0, y: 30 },
  visible: (i) => ({
    opacity: 1,
    y: 0,
    transition: { type: "spring", stiffness: 100, damping: 20, delay: i * 0.1 },
  }),
};

export default function FeatureGrid() {
  return (
    <section id="features" className="relative px-6 py-24 max-w-6xl mx-auto">
      <motion.h2
        initial={{ opacity: 0, y: 20 }}
        whileInView={{ opacity: 1, y: 0 }}
        viewport={{ once: true }}
        transition={{ type: "spring", stiffness: 100, damping: 20 }}
        className="text-3xl sm:text-4xl font-bold tracking-tight text-center mb-16 text-white/90"
      >
        Cheat Features
      </motion.h2>

      <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-5">
        {features.map((f, i) => (
          <motion.div
            key={f.title}
            custom={i}
            initial="hidden"
            whileInView="visible"
            viewport={{ once: true, amount: 0.2 }}
            variants={cardVariants}
            whileHover={{ scale: 1.03, rotateX: 2, rotateY: -2 }}
            transition={{ type: "spring", stiffness: 200, damping: 15 }}
            style={{ perspective: 800 }}
          >
            <GlassContainer
              animate={false}
              className="p-6 h-full flex flex-col gap-4 cursor-default"
            >
              <div className="w-10 h-10 rounded-xl bg-white/[0.06] border border-white/[0.08] flex items-center justify-center">
                <f.icon className="w-5 h-5 text-white/70" strokeWidth={1.5} />
              </div>
              <h3 className="text-lg font-semibold text-white/90">{f.title}</h3>
              <p className="text-sm leading-relaxed text-white/40">{f.desc}</p>
            </GlassContainer>
          </motion.div>
        ))}
      </div>
    </section>
  );
}
