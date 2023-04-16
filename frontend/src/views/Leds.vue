<script lang="ts" setup>
import { onMounted, ref } from "vue";
import apiService from "@/ApiService";
import { Led } from "@/ApiService";
import useAppStore from "@/store/app";
const { publishAlert } = useAppStore();

const leds = ref<Led[]>([]);

async function updateLedData() {
   try {
      const response = await apiService.getStatus();
      leds.value = response.data.leds;
   } catch (error) {
      console.log("error", error);
      publishAlert(
         "error",
         "Could not load status data",
         JSON.stringify(error)
      );
   }
}
async function onSwitchChange(id: number, isOn: boolean | undefined) {
   try {
      if (isOn === undefined) return;
      const response = await apiService.updateLedStatus({ id, isOn });
   } catch (error) {
      console.log("error", error);
      publishAlert(
         "error",
         `Could not update led ${id} isOn to ${isOn}`,
         JSON.stringify(error)
      );
   }
}
onMounted(async () => {
   await updateLedData();
});
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
                     color="secondary"
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
