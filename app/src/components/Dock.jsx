import { motion } from "framer-motion";
import {
  Home,
  Grid3X3,
  Activity,
  Shield,
  Download,
  User,
} from "lucide-react";

const links = [
  { icon: Home, label: "Home", href: "#hero" },
  { icon: Grid3X3, label: "Features", href: "#features" },
  { icon: Activity, label: "Status", href: "#status" },
  { icon: Shield, label: "Security", href: "#security" },
  { icon: Download, label: "Download", href: "#download" },
  { icon: User, label: "Cabinet", href: "https://cab.mozority.pro/p/cabinet", external: true },
];

export default function Dock() {
  return (
    <motion.nav
      initial={{ y: 80, opacity: 0 }}
      animate={{ y: 0, opacity: 1 }}
      transition={{ type: "spring", stiffness: 100, damping: 20, delay: 0.5 }}
      className="fixed bottom-6 left-1/2 -translate-x-1/2 z-50"
    >
      <div className="flex items-center gap-1 sm:gap-2 px-4 py-3 rounded-2xl backdrop-blur-[20px] bg-black/40 border border-white/[0.1] shadow-[0_8px_32px_0_rgba(0,0,0,0.8)]">
        {links.map((link) => {
          const { icon: Icon, label, href } = link;
          return (
          <motion.a
            key={label}
            href={href}
            {...(link.external ? { target: "_blank", rel: "noopener noreferrer" } : {})}
            whileHover={{ scale: 1.25, y: -4 }}
            whileTap={{ scale: 0.95 }}
            transition={{ type: "spring", stiffness: 300, damping: 15 }}
            className="group relative flex items-center justify-center w-11 h-11 rounded-xl hover:bg-white/[0.08] transition-colors"
            aria-label={label}
          >
            <Icon className="w-5 h-5 text-white/60 group-hover:text-white transition-colors" strokeWidth={1.5} />
            {/* Tooltip */}
            <span className="absolute -top-9 left-1/2 -translate-x-1/2 px-2 py-1 text-[11px] rounded-md bg-white/10 backdrop-blur-md text-white/80 opacity-0 group-hover:opacity-100 transition-opacity whitespace-nowrap pointer-events-none">
              {label}
            </span>
          </motion.a>
          );
        })}
      </div>
    </motion.nav>
  );
}
