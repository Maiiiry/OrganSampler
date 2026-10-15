"use strict";
const config = window.organSamplerSite || {};
const views = {
  main: { file: "main.png", alt: "The main window: top bar, combination pistons, the global sound sliders with the polyphony display and the stop grids of four divisions.", caption: "The console: one column per division, the stops as buttons, couplers and tremulants in the headers, pistons and the crescendo above." },
  stoplist: { file: "stoplist.png", alt: "The Stop List: a table of stops per division with their sample folders per perspective and the transmissions.", caption: "The Stop List: the organ is built here from the loaded folder trees - stops, perspectives, divisions, transmissions." },
  divisions: { file: "divisions.png", alt: "The Divisions dialog: MIDI, compass, swell, tremulant and wind of a division, and its audio tabs with the device, buffer size and output channels per perspective.", caption: "The Divisions dialog: MIDI and compass, swell, tremulant, wind - and one audio route per perspective, each on any device and channels." },
  editor: { file: "editor.png", alt: "The Configuration Editor: the engine, global sound and console options, each with a question mark that opens the manual at its chapter.", caption: "The Configuration Editor: every option explained by the manual chapter behind its question mark." }
};
for (const button of document.querySelectorAll("[data-view]")) {
  button.addEventListener("click", () => {
    const view = views[button.dataset.view];
    if (!view) return;
    for (const peer of document.querySelectorAll("[data-view]")) peer.setAttribute("aria-pressed", String(peer === button));
    const img = document.getElementById("screenshot");
    img.src = "assets/" + view.file;
    img.alt = view.alt;
    for (const id of ["screenshot-link", "full-size"]) document.getElementById(id).href = img.src;
    document.getElementById("screenshot-caption").textContent = view.caption;
  });
}
for (const el of document.querySelectorAll("[data-version]")) el.textContent = config.version || "";
const validRepository = typeof config.repository === "string" && /^[A-Za-z0-9][A-Za-z0-9-]*\/[A-Za-z0-9_.-]+$/.test(config.repository);
const repo = validRepository ? "https://github.com/" + config.repository : "https://github.com/zurek-jiri/OrganSampler";
for (const el of document.querySelectorAll("[data-repo-path]")) el.href = repo + "/blob/main/" + el.dataset.repoPath;
for (const el of document.querySelectorAll("[data-repo-tree]")) el.href = repo + "/tree/main/" + el.dataset.repoTree;
for (const el of document.querySelectorAll("[data-repo-suffix]")) el.href = repo + el.dataset.repoSuffix;

// The organ configurations: configurations/list.json is written when the site
// is deployed (one zip per configuration folder beside it).
const configurationList = document.querySelector("[data-configuration-list]");
if (configurationList) {
  const showNote = (text) => { configurationList.textContent = ""; const p = document.createElement("p"); p.className = "small"; p.textContent = text; configurationList.appendChild(p); };
  fetch("configurations/list.json", { cache: "no-cache" })
    .then((response) => (response.ok ? response.json() : []))
    .then((items) => {
      if (!Array.isArray(items) || items.length === 0) { showNote("No configuration published yet."); return; }
      configurationList.textContent = "";
      for (const item of items) {
        const article = document.createElement("article");
        const h3 = document.createElement("h3");
        h3.textContent = item.title || item.folder;
        const summary = document.createElement("p");
        summary.textContent = item.summary || "";
        const links = document.createElement("p");
        links.className = "links";
        const download = document.createElement("a");
        download.className = "button primary";
        download.href = item.zip;
        download.setAttribute("download", "");
        download.textContent = "Download" + (item.bytes ? " (" + Math.max(1, Math.round(item.bytes / 1024)) + " KB)" : "");
        const readme = document.createElement("a");
        readme.className = "text-link";
        readme.href = item.readme;
        readme.textContent = "README ↗";
        links.append(download, " ", readme);
        article.append(h3, summary, links);
        configurationList.appendChild(article);
      }
    })
    .catch(() => showNote("The list of configurations could not be loaded; see the configurations folder on GitHub."));
}
