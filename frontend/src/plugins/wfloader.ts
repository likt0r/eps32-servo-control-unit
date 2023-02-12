/**
 * plugins/webfontloader.ts
 *
 * webfontloader documentation: https://github.com/typekit/webfontloader
 */
import webFontLoader from "webfontloader";

export async function loadFonts() {
  webFontLoader.load({
    google: {
      families: ["Roboto:100,300,400,500,700,900&display=swap"],
    },
  });
}
