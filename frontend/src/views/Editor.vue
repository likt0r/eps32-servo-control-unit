<script lang="ts">
import { defineComponent, ComponentPublicInstance, ref } from "vue";
import apiInstance from "@/Api";
import { Servo } from "@/Api";
import { mdiAngleAcute, mdiSpeedometer } from "@mdi/js";

interface IInstance extends ComponentPublicInstance {
   setServoData(data: Servo[]): void;
}

export default defineComponent({
   async beforeRouteEnter(to, from, next) {
      try {
         const data = await apiInstance.getServos();
         next((vm) => {
            const instance = vm as IInstance;
            instance.setServoData(data);
         });
      } catch (error) {
         console.log("error", error);
         next(false);
      }
   },
});
</script>

<script lang="ts" setup>
const servos = ref<Servo[]>([]);
const motionSpeed = ref<number>(0.1);
const setServoData = (data: Servo[]) => {
   servos.value = data;
};

async function updateLedData() {
   try {
      servos.value = await apiInstance.getServos();
   } catch (error) {
      console.log("error", error);
   }
}
async function onPositionUpdate(id: number, position: number) {
   try {
      await apiInstance.setServoPosition(id, position);
   } catch (error) {
      console.log("error", error);
   }
}

async function onMotionSpeedUpdate(speed: number) {
   try {
      await apiInstance.setMotionSpeed(speed);
   } catch (error) {
      console.log("error", error);
   }
}

defineExpose({ setServoData });
</script>

<template>
   <v-card flat>
      <v-card-text>
         <v-container fluid>
            <v-row>
               <v-col cols="12" sm="10" md="10">
                  <div class="text-caption">
                     Motion Speed: {{ motionSpeed }}
                  </div>

                  <v-slider
                     v-model="motionSpeed"
                     :min="0"
                     :max="3"
                     :step="0.05"
                     @update:model-value="onMotionSpeedUpdate"
                     thumb-label
                     :prepend-icon="mdiSpeedometer"
                  ></v-slider>
               </v-col>
               <v-col cols="12" sm="2" md="2">
                  <v-text-field
                     v-model="motionSpeed"
                     type="number"
                     :step="0.05"
                     variant="outlined"
                     hide-details="auto"
                  ></v-text-field>
               </v-col>
            </v-row>
            <v-row v-for="(servo, index) in servos" :key="servo.id">
               <v-col cols="12" sm="10" md="10">
                  <div class="text-caption">Servo Pin: {{ servo.pin }}</div>

                  <v-slider
                     v-model="servo.position"
                     :min="servo.minAngle"
                     :max="servo.maxAngle"
                     :step="0.1"
                     thumb-label
                     @update:model-value="onPositionUpdate(servo.id, $event)"
                     :prepend-icon="mdiAngleAcute"
                  ></v-slider>
               </v-col>
               <v-col cols="12" sm="2" md="2">
                  <v-text-field
                     v-model="servo.position"
                     type="number"
                     variant="outlined"
                     hide-details="auto"
                  ></v-text-field>
               </v-col>
            </v-row>
         </v-container>
      </v-card-text>
   </v-card>
</template>
