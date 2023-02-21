<script lang="ts">
import { defineComponent, ComponentPublicInstance, ref } from "vue";
import apiInstance from "@/Api";
import { Led } from "@/Api";

interface IInstance extends ComponentPublicInstance {
   setLedData(data: Led[]): void;
}

export default defineComponent({
   async beforeRouteEnter(to, from, next) {
      try {
         const data = await apiInstance.getLeds();
         next((vm) => {
            const instance = vm as IInstance;
            instance.setLedData(data);
         });
      } catch (error) {
         console.log("error", error);
         next(false);
      }
   },
});
</script>

<script lang="ts" setup>
const leds = ref<Led[]>([]);

const setLedData = (data: Led[]) => {
   leds.value = data;
   console.log("vue-route::from::", leds);
};

async function updateLedData() {
   try {
      leds.value = await apiInstance.getLeds();
   } catch (error) {
      console.log("error", error);
   }
}
async function onSwitchChange(id: number, isOn: boolean | undefined) {
   try {
      if (isOn === undefined) return;
      await apiInstance.switchLed(id, isOn);
   } catch (error) {
      console.log("error", error);
   }
}

defineExpose({ setLedData });
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
                     :label="'Pin ' + led.pin"
                     color="red"
                     v-model="led.isOn"
                     @update:model-value="onSwitchChange(led.id, $event)"
                     hide-details
                  ></v-switch>
               </v-col>
            </v-row>
         </v-container>
      </v-card-text>
   </v-card>
</template>
