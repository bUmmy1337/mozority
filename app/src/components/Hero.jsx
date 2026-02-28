import { motion } from "framer-motion";

const spring = { type: "spring", stiffness: 100, damping: 20 };

export default function Hero() {
  return (
    <section
      id="hero"
      className="relative min-h-screen flex flex-col items-center justify-center px-6 text-center"
    >
      {/* Subtle radial glow behind title */}
      <div className="absolute w-[600px] h-[600px] rounded-full bg-white/[0.02] blur-[120px] pointer-events-none" />

      <motion.h1
        initial={{ opacity: 0, y: 40 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ ...spring, delay: 0.2 }}
        className="text-6xl sm:text-8xl md:text-9xl font-extrabold tracking-[-0.05em] text-white select-none"
      >
        MOZORITY
      </motion.h1>

      <motion.p
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ ...spring, delay: 0.4 }}
        className="mt-4 text-lg sm:text-xl text-white/50 font-light tracking-wide"
      >
        Precision. Stealth. Performance.
      </motion.p>

      <div className="flex items-center gap-4 mt-10">
        <motion.a
          href="https://cab.mozority.pro/p/cabinet"
          target="_blank"
          rel="noopener noreferrer"
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ ...spring, delay: 0.6 }}
          whileHover={{ scale: 1.05 }}
          whileTap={{ scale: 0.97 }}
          className="relative inline-flex items-center px-8 py-3.5 rounded-full bg-white/[0.08] border border-white/[0.12] text-white font-medium overflow-hidden group cursor-pointer backdrop-blur-sm"
        >
          <span className="absolute inset-0 -translate-x-full group-hover:translate-x-full transition-transform duration-700 ease-in-out bg-gradient-to-r from-transparent via-white/20 to-transparent" />
          <span className="relative z-10">Get Access</span>
        </motion.a>

        <motion.a
          href="https://cab.mozority.pro/p/cabinet"
          target="_blank"
          rel="noopener noreferrer"
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ ...spring, delay: 0.75 }}
          whileHover={{ scale: 1.05 }}
          whileTap={{ scale: 0.97 }}
          className="inline-flex items-center px-6 py-3.5 rounded-full text-white/50 font-medium hover:text-white/80 transition-colors cursor-pointer"
        >
          Cabinet &rarr;
        </motion.a>
      </div>

      {/* Scroll indicator */}
      <motion.div
        initial={{ opacity: 0 }}
        animate={{ opacity: 1 }}
        transition={{ delay: 1.2 }}
        className="absolute bottom-28 flex flex-col items-center gap-2"
      >
        <motion.div
          animate={{ y: [0, 8, 0] }}
          transition={{ duration: 1.8, repeat: Infinity, ease: "easeInOut" }}
          className="w-5 h-8 rounded-full border border-white/20 flex items-start justify-center p-1"
        >
          <div className="w-1 h-2 rounded-full bg-white/40" />
        </motion.div>
      </motion.div>
    </section>
  );
}
