// Utilities
import { defineStore } from "pinia";
import { ref, computed } from "vue";
import { nanoid } from "nanoid";
export type AlertContent = {
   id: string;
   type: "success" | "error" | "info" | "warning";
   text: string;
   title: string;
};
export const useAppStore = defineStore("app", () => {
   const drawerVisible = ref(false);
   const isDarkMode = ref(false);
   const alertContents = ref<AlertContent[]>([]);

   const theme = computed(() => (isDarkMode.value ? "dark" : "light"));

   function setDarkMode(value: boolean) {
      isDarkMode.value = value;
   }
   function setDrawer(value: boolean) {
      drawerVisible.value = value;
   }
   function toggleDrawer() {
      drawerVisible.value = !drawerVisible.value;
   }
   function toggleDarkMode() {
      isDarkMode.value = !isDarkMode.value;
   }

   function publishAlert(
      type: "success" | "error" | "info" | "warning",
      text: string,
      title: string
   ) {
      alertContents.value.push({
         id: nanoid(),
         type,
         text,
         title,
      });
      setTimeout(() => {
         unpublishAlert(alertContents.value[0].id);
      }, 5000);
   }
   function unpublishAlert(id: string) {
      console.log("unpublishAlert", id);
      alertContents.value = alertContents.value.filter((a) => a.id !== id);
   }
   return {
      drawerVisible,
      isDarkMode,
      theme,
      setDarkMode,
      setDrawer,
      toggleDrawer,
      toggleDarkMode,
      alertContents,
      publishAlert,
      unpublishAlert,
   };
});

export default useAppStore;
