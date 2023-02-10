// Utilities
import { defineStore } from "pinia";
import { ref, computed } from "vue";
export const useAppStore = defineStore("app", () => {
  const drawerVisible = ref(false);
  const isDarkMode = ref(false);

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
  return {
    drawerVisible,
    isDarkMode,
    theme,
    setDarkMode,
    setDrawer,
    toggleDrawer,
    toggleDarkMode,
  };
});

export default useAppStore;