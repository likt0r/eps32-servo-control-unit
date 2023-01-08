import * as fs from "fs/promises";
import * as path from "path";
import { existsSync } from "fs";
import { lookup } from "mime-types";

const buildFolder = "dist";
const dataFolder = "../data";
const libFolder = "../lib/frontend-files";
// read build folder recursively and get all files
async function walk(dir) {
  const entries = await fs.readdir(dir);
  let ret: string[] = [];
  for (const entry of entries) {
    const fullPath = path.join(dir, entry) as string;
    const info = await fs.stat(fullPath);
    if (info.isDirectory()) {
      ret = [...ret, ...(await walk(fullPath))];
    } else {
      ret = [...ret, fullPath];
    }
  }

  return ret.map((file) => file.replace(buildFolder, ""));
}

(async function () {
  // get all files in build folder
  const files = await walk(buildFolder);
  const fileInfos = files.map((file) => ({
    fileUrl: file,
    filepath: file.replace(/\//g, "_").replace(/^_/, "/"),
    mimeType: lookup(file),
  }));

  console.log(fileInfos);
  // empty data folder
  if (existsSync(dataFolder))
    await fs.rm(dataFolder, { recursive: true, force: true });
  // create data
  await fs.mkdir(dataFolder, { recursive: true });

  // copy files to data folder
  for (const fileInfo of fileInfos) {
    await fs.copyFile(
      path.join(buildFolder, fileInfo.fileUrl),
      path.join(dataFolder, fileInfo.filepath)
    );
  }
  // check if lib folder exists
  if (existsSync(libFolder))
    await fs.rm(libFolder, { recursive: true, force: true });
  // create data
  await fs.mkdir(libFolder, { recursive: true });

  // create c++ struct array for files

  const fileStructs = fileInfos.map(
    (fileInfo) => `{"${fileInfo.fileUrl}", "${fileInfo.filepath}", "${fileInfo.mimeType}"}`
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

  await fs
    .writeFile(path.join(libFolder, "FrontendFiles.h"), fileStructsArray)
    .catch((err) => console.log(err));
})();
