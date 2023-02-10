// Utilities
import { defineStore } from "pinia";
import { ref, computed } from "vue";
export interface Led {
    id: number;
    pin: number;
    on: boolean;
    pwm: number;
}
export const useAppStore = defineStore("leds", () => {
  const drawerVisible = ref<Led[]>([{
    id:1,
    pin: 13,
    on: false,
    pwm: 0,
  },{
    id:2,
    pin: 14,
    on: false,
    pwm: 0,
  }]);
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