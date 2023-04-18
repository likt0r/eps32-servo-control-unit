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
      <v-card-title> Leds </v-card-title>
      <v-card-text>
         <EditableList
            v-model="data"
            :id-function="(item) => item.id"
            :title-function="(item) => `Led ID: ${item.id} on Pin: ${item.pin}`"
            :subtitle-function="(item) => `Default isOn: ${item.isOn}`"
            :form-component="LedForm"
            :default-data="defaultLed"
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

import LedForm from "./LedForm.vue";
import EditableList from "@/components/EditableList/index.vue";
import apiService from "@/ApiService";
import { Led } from "@/ApiService";
import useAppStore from "@/store/app";
const { publishAlert } = useAppStore();

const data = ref<Led[]>([]);
const loading = ref(false);
const dataChanged = ref(false);
const defaultLed = ref<Led>({
   id: 0,
   pin: 0,
   isOn: false,
});
async function onChange(
   type: "delete" | "add" | "edit",
   oldValue: Led[] | null,
   newValue: Led[] | null
) {
   console.log("onChange: " + type, oldValue);
   dataChanged.value = true;
}
async function onSave() {
   try {
      loading.value = true;
      await apiService.setLeds(data.value);
      dataChanged.value = false;
   } catch (error: any) {
      console.log("error", error);
      publishAlert("error", "Could not load Leds", JSON.stringify(error));
   }
   loading.value = false;
}

onMounted(async () => {
   loading.value = true;
   try {
      loading.value = true;
      const response = await apiService.getLeds();
      data.value = response.data;
   } catch (error: any) {
      console.log("error", error);
      publishAlert("error", "Could not load Leds", JSON.stringify(error));
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
