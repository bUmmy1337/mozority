import { useState } from "react";
import { motion } from "framer-motion";
import GlassContainer from "./GlassContainer";
import { Crosshair, Eye, Paintbrush, Moon, Zap, Move } from "lucide-react";

const features = [
  {
    icon: Paintbrush,
    title: "Skin Changer",
    desc: "Complete inventory replacement — weapons, knives, gloves, agents. Customization of wear, pattern, StatTrak.",
    img: "/img/skinchanger.png",
  },
  {
    icon: Eye,
    title: "ESP",
    desc: "Boxes, health bars, player names, real-time previews.",
    img: "/img/esp.png",
  },
  {
    icon: Crosshair,
    title: "Aimbot",
    desc: "Configurable aimbot with smoothness. Unsafe feature — must be enabled in settings.",
    img: "/img/aimbot.png",
  },
  {
    icon: Moon,
    title: "Nightmode",
    desc: "Recoloring world materials (night mode).",
    img: "/img/nightmode.png",
  },
  {
    icon: Zap,
    title: "Chams",
    desc: "Highlighting player models through walls.",
    img: "/img/chams.png",
  },
  {
    icon: Move,
    title: "Movement",
    desc: "BunnyHop and other assists. Also requires enabling unsafe features.",
    img: "/img/movement.png",
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

function FeatureCard({ f, i }) {
  const [isFlipped, setIsFlipped] = useState(false);

  return (
    <motion.div
      custom={i}
      initial="hidden"
      whileInView="visible"
      viewport={{ once: true, amount: 0.2 }}
      variants={cardVariants}
      onHoverStart={() => setIsFlipped(true)}
      onHoverEnd={() => setIsFlipped(false)}
      style={{ perspective: 1000 }}
      className="cursor-pointer h-[220px]"
    >
      <motion.div
        animate={{ rotateY: isFlipped ? 165 : 0 }}
        transition={{ type: "spring", stiffness: 100, damping: 20 }}
        style={{ transformStyle: "preserve-3d" }}
        className="relative w-full h-full"
      >
        {/* Front face */}
        <div
          className="absolute inset-0"
          style={{ backfaceVisibility: "hidden" }}
        >
          <GlassContainer
            animate={false}
            className="p-6 h-full flex flex-col gap-4"
          >
            <div className="w-10 h-10 rounded-xl bg-white/[0.06] border border-white/[0.08] flex items-center justify-center">
              <f.icon className="w-5 h-5 text-white/70" strokeWidth={1.5} />
            </div>
            <h3 className="text-lg font-semibold text-white/90">{f.title}</h3>
            <p className="text-sm leading-relaxed text-white/40">{f.desc}</p>
          </GlassContainer>
        </div>

        {/* Back face (preview image) */}
        <div
          className="absolute inset-0"
          style={{
            backfaceVisibility: "hidden",
            transform: "rotateY(180deg)",
          }}
        >
          <div className="h-full rounded-2xl overflow-hidden backdrop-blur-[20px] bg-black/40 border border-white/[0.1] shadow-[0_8px_32px_0_rgba(0,0,0,0.8)]">
            <img
              src={f.img}
              alt={`${f.title} preview`}
              className="w-full h-full object-cover opacity-90"
              loading="lazy"
            />
            <div className="absolute inset-0 bg-gradient-to-t from-black/60 via-transparent to-transparent" />
            <div className="absolute bottom-4 left-4">
              <span className="text-sm font-semibold text-white/90">{f.title}</span>
            </div>
          </div>
        </div>
      </motion.div>
    </motion.div>
  );
}

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
          <FeatureCard key={f.title} f={f} i={i} />
        ))}
      </div>
    </section>
  );
}
