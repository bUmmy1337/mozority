import { motion } from "framer-motion";

export default function GlassContainer({
  children,
  className = "",
  variant = "light",
  animate = true,
  ...props
}) {
  const base =
    variant === "dark"
      ? "bg-black/40 border border-white/[0.08] shadow-[0_8px_32px_0_rgba(0,0,0,0.8)]"
      : "bg-white/[0.05] border border-white/[0.1] shadow-[0_8px_32px_0_rgba(0,0,0,0.8)]";

  const Comp = animate ? motion.div : "div";
  const motionProps = animate
    ? {
        initial: { opacity: 0, y: 20 },
        whileInView: { opacity: 1, y: 0 },
        viewport: { once: true, amount: 0.2 },
        transition: { type: "spring", stiffness: 100, damping: 20 },
      }
    : {};

  return (
    <Comp
      className={`backdrop-blur-[20px] rounded-2xl ${base} ${className}`}
      {...motionProps}
      {...props}
    >
      {children}
    </Comp>
  );
}
