import MeshBackground from "./components/MeshBackground";
import Dock from "./components/Dock";
import Hero from "./components/Hero";
import FeatureGrid from "./components/FeatureGrid";
import Security from "./components/Security";
import StatusWidget from "./components/StatusWidget";
import DownloadSection from "./components/DownloadSection";

export default function App() {
  return (
    <>
      <MeshBackground />
      <Dock />
      <main className="relative z-10">
        <Hero />
        <FeatureGrid />
        <Security />
        <StatusWidget />
        <DownloadSection />
        {/* Spacer for bottom dock */}
        <div className="h-24" />
      </main>
    </>
  );
}
