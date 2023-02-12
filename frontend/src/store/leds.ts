// Utilities
import { defineStore } from "pinia";
import { ref, computed } from "vue";
export interface Led {
  id: number;
  pin: number;
  isOn: boolean;
  pwm: number;
}
export const useLedStore = defineStore("leds", () => {
  const leds = ref<Led[]>([
    {
      id: 1,
      pin: 13,
      isOn: false,
      pwm: 0,
    },
    {
      id: 2,
      pin: 14,
      isOn: false,
      pwm: 0,
    },
  ]);
  function toggleLed(idx: number) {
    const element = leds.value.find((led) => {
      return idx === led.id;
    });
    if (element) {
      element.isOn = !element.isOn;
    }
  }
  function setLed(idx: number, isOn: boolean) {
    const element = leds.value.find((led) => {
      return idx === led.id;
    });
    if (element) {
      element.isOn = isOn;
    }
  }

  return {
    leds,
    toggleLed,
    setLed,
  };
});

export default useLedStore;
