import { motion } from "framer-motion";

const blobs = [
  {
    size: "w-[600px] h-[600px]",
    color: "bg-[#1a1a1a]",
    initial: { x: -200, y: -100 },
    animate: { x: [-200, 100, -50, -200], y: [-100, 150, -200, -100] },
    duration: 25,
  },
  {
    size: "w-[500px] h-[500px]",
    color: "bg-[#0b0b0b]",
    initial: { x: 300, y: 200 },
    animate: { x: [300, -100, 200, 300], y: [200, -50, 300, 200] },
    duration: 30,
  },
  {
    size: "w-[450px] h-[450px]",
    color: "bg-[#1a1a1a]",
    initial: { x: 0, y: 400 },
    animate: { x: [0, 250, -150, 0], y: [400, 100, 350, 400] },
    duration: 28,
  },
  {
    size: "w-[550px] h-[550px]",
    color: "bg-[#0b0b0b]",
    initial: { x: -300, y: 300 },
    animate: { x: [-300, 50, -200, -300], y: [300, -100, 200, 300] },
    duration: 32,
  },
];

export default function MeshBackground() {
  return (
    <div className="fixed inset-0 -z-10 overflow-hidden bg-black">
      {/* Static mesh gradient base */}
      <div
        className="absolute inset-0"
        style={{
          background:
            "radial-gradient(ellipse at 20% 50%, #1a1a1a 0%, transparent 50%), radial-gradient(ellipse at 80% 20%, #0b0b0b 0%, transparent 50%), radial-gradient(ellipse at 50% 80%, #1a1a1a 0%, transparent 50%), #000000",
        }}
      />
      {/* Liquid blobs */}
      {blobs.map((blob, i) => (
        <motion.div
          key={i}
          className={`absolute rounded-full ${blob.size} ${blob.color} opacity-60 blur-[120px]`}
          initial={blob.initial}
          animate={blob.animate}
          transition={{
            duration: blob.duration,
            repeat: Infinity,
            ease: "easeInOut",
          }}
        />
      ))}
    </div>
  );
}
