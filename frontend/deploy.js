const fs = require("fs");
const path = require("path");
const { existsSync } = require("fs");
const { lookup } = require("mime-types");
const { nanoid } = require("nanoid");
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
   const fileInfos = files
      .map((file) => {
         const isGzipped = /\.gz$/.test(file);
         const fileUrl = isGzipped ? file.replace(/\.gz$/, "") : file;
         return {
            fileUrl,
            sourceFilepath: file,
            targetFilepath: "/" + nanoid(10),
            mimeType: lookup(fileUrl),
            isGzipped,
         };
      })
      .reduce((acc, fileInfo) => {
         acc.push(fileInfo);
         if (fileInfo.fileUrl === "/index.html") {
            // add index.html is also served on /
            acc.push({
               fileUrl: "/",
               sourceFilepath: fileInfo.sourceFilepath,
               targetFilepath: fileInfo.targetFilepath,
               mimeType: fileInfo.mimeType,
               isGzipped: fileInfo.isGzipped,
            });
         }
         return acc;
      }, []);
   console.log(fileInfos);
   // empty data folder
   if (existsSync(dataFolder))
      fs.rmSync(dataFolder, { recursive: true, force: true });
   // create data
   fs.mkdirSync(dataFolder, { recursive: true });

   // copy files to data folder
   for (const fileInfo of fileInfos) {
      fs.copyFileSync(
         path.join(buildFolder, fileInfo.sourceFilepath),
         path.join(dataFolder, fileInfo.targetFilepath)
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
         `{"${fileInfo.fileUrl}", "${fileInfo.targetFilepath}", "${fileInfo.mimeType}", ${fileInfo.isGzipped}}`
   );
   const fileStructsStr = fileStructs.join(",\n");
   const fileStructsArray = `
struct FrontendFileInfo{
    std::string url;
    std::string filepath;
    std::string mimeType;
    bool isGzipped;  
};
const int frontendFilesCount = ${fileStructs.length};
const FrontendFileInfo frontendFileInfos[${fileStructs.length}] = {\n${fileStructsStr}\n};`;
   // write c++ struct array to file

   fs.writeFileSync(path.join(libFolder, "FrontendFiles.h"), fileStructsArray);
})();
