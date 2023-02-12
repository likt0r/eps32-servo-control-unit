<script lang="ts" setup>
import { computed } from "vue";
import { storeToRefs } from "pinia";
import useLedStore from "@/store/leds";

const { leds } = storeToRefs(useLedStore());
const { toggleLed, setLed } = useLedStore();
const ledsValues = leds.value.map((led) => ({
  get() {
    return led.isOn;
  },
  set(value: boolean) {
    console.log(value);
    setLed(led.id, value);
  },
}));
</script>

<template>
  <v-card flat>
    <v-card-text>
      <v-container fluid>
        <v-row>
          <v-col cols="12" sm="4" md="4">
            <v-switch
              v-for="(led, index) in leds"
              :key="led.id"
              :label="'id ' + led.isOn"
              v-model="ledsValues[index]"
              color="red"
              hide-details
            ></v-switch>
          </v-col>
        </v-row>
      </v-container>
    </v-card-text>
  </v-card>
</template>
