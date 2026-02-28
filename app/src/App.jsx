import MeshBackground from "./components/MeshBackground";
import Dock from "./components/Dock";
import Hero from "./components/Hero";
import FeatureGrid from "./components/FeatureGrid";
import StatusWidget from "./components/StatusWidget";

export default function App() {
  return (
    <>
      <MeshBackground />
      <Dock />
      <main className="relative z-10">
        <Hero />
        <FeatureGrid />
        <StatusWidget />
        {/* Spacer for bottom dock */}
        <div className="h-24" />
      </main>
    </>
  );
}
