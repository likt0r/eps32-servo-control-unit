const fs = require("fs");
const path = require("path");
const { existsSync } = require("fs");
const { lookup } = require("mime-types");

const buildFolder = "dist";
const dataFolder = "../data";
const libFolder = "../lib/frontend-files";
// read build folder recursively and get all files
function walk(dir) {
   const entries = fs.readdirSync(dir);
   let ret = [];
   for (const entry of entries) {
      const fullPath = path.join(dir, entry);
      const info = fs.statSync(fullPath);
      if (info.isDirectory()) {
         ret = [...ret, ...walk(fullPath)];
      } else {
         ret = [...ret, fullPath];
      }
   }

   return ret.map((file) => file.replace(buildFolder, ""));
}

(function () {
   // get all files in build folder
   const files = walk(buildFolder);
   const fileInfos = files.map((file) => ({
      fileUrl: file,
      filepath: file.replace(/\//g, "_").replace(/^_/, "/"),
      mimeType: lookup(file),
   }));

   console.log(fileInfos);
   // empty data folder
   if (existsSync(dataFolder))
      fs.rmSync(dataFolder, { recursive: true, force: true });
   // create data
   fs.mkdirSync(dataFolder, { recursive: true });

   // copy files to data folder
   for (const fileInfo of fileInfos) {
      fs.copyFileSync(
         path.join(buildFolder, fileInfo.fileUrl),
         path.join(dataFolder, fileInfo.filepath)
      );
   }
   // check if lib folder exists
   if (existsSync(libFolder))
      fs.rmSync(libFolder, { recursive: true, force: true });
   // create data
   fs.mkdirSync(libFolder, { recursive: true });

   // create c++ struct array for files

   const fileStructs = fileInfos.map(
      (fileInfo) =>
         `{"${fileInfo.fileUrl}", "${fileInfo.filepath}", "${fileInfo.mimeType}"}`
   );
   const fileStructsStr = fileStructs.join(",\n");
   const fileStructsArray = `
struct FrontendFileInfo{
    std::string url;
    std::string filepath;
    std::string mimeType;
  
};
const int frontendFilesCount = ${fileStructs.length};
const FrontendFileInfo frontendFileInfos[${fileStructs.length}] = {\n${fileStructsStr}\n};`;
   // write c++ struct array to file

   fs.writeFileSync(path.join(libFolder, "FrontendFiles.h"), fileStructsArray);
})();
