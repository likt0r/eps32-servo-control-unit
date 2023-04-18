<template>
   <v-card>
      <template v-slot:loader="{ isActive }">
         <v-progress-linear
            :active="loading"
            color="primary"
            height="8"
            indeterminate
         ></v-progress-linear>
      </template>
      <v-card-title> Servos </v-card-title>
      <v-card-text>
         <EditableList
            v-model="data"
            :id-function="(item) => item.id"
            :title-function="
               (item) => `Servo ID: ${item.id} on Pin: ${item.pin}`
            "
            :subtitle-function="
               (item) =>
                  `Start Position: ${item.position} | Min PWM: ${item.minPwm} | Max PWM: ${item.maxPwm} | Min Angle: ${item.minAngle} | Max Angle: ${item.maxAngle}`
            "
            :form-component="ServoForm"
            :default-data="defaultServo"
            @changed="onChange"
            :disabled="loading"
         />
      </v-card-text>
      <v-card-actions>
         <v-spacer></v-spacer>
         <v-btn
            @click="onSave"
            color="primary"
            :disabled="!dataChanged || loading"
         >
            Save
         </v-btn>
      </v-card-actions>
   </v-card>
</template>
<script lang="ts" setup>
import { onMounted, ref, computed } from "vue";

import ServoForm from "./ServoForm.vue";
import EditableList from "@/components/EditableList/index.vue";
import apiService from "@/ApiService";
import { Servo } from "@/ApiService";
import useAppStore from "@/store/app";
const { publishAlert } = useAppStore();

const data = ref<Servo[]>([]);
const loading = ref(false);
const dataChanged = ref(false);
const defaultServo = ref<Servo>({
   id: 0,
   pin: 0,
   position: 0,
   minPwm: 0,
   maxPwm: 680,
   minAngle: 0,
   maxAngle: 180,
});
async function onChange(
   type: "delete" | "add" | "edit",
   oldValue: Servo[],
   newValue: Servo[]
) {
   console.log("onChange: " + type, oldValue);
   dataChanged.value = true;
}
async function onSave() {
   loading.value = true;
   await apiService.setServos(data.value);
   loading.value = false;
   dataChanged.value = false;
}

onMounted(async () => {
   loading.value = true;
   try {
      loading.value = true;
      const response = await apiService.getServos();
      data.value = response.data;
   } catch (error: any) {
      console.log("error", error);
      publishAlert(
         "error",
         "Could not load WiFi-Credentials",
         JSON.stringify(error)
      );
   }
   loading.value = false;
});
</script>

<style scoped>
.ghost {
   opacity: 0.5;
   background: #c8ebfb;
}

.list-group-item {
   cursor: move;
}
</style>
