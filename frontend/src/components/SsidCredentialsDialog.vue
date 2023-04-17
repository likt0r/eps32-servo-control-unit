<template>
   <v-dialog v-model="dialog" :persistent="persistent" width="1024">
      <template v-slot:activator="{ props }">
         <v-btn
            :icon="buttonIcon"
            color="primary"
            v-bind="props"
            variant="text"
         ></v-btn>
      </template>
      <v-card>
         <v-form validate-on="submit" @submit.prevent="submit">
            <v-card-title>
               <span class="text-h5">Add WiFi-Credentials</span>
            </v-card-title>
            <v-card-text>
               <v-container>
                  <v-row>
                     <v-col cols="12" sm="12" md="6">
                        <v-text-field
                           v-model="ssid"
                           label="SSID"
                           required
                           validate-on="submit"
                           :rules="[isRequired('SSID')]"
                        ></v-text-field>
                     </v-col>
                     <v-col cols="12" sm="12" md="6">
                        <v-text-field
                           v-model="password"
                           label="Password"
                           :type="showPassword ? 'text' : 'password'"
                           :append-icon="showPassword ? mdiEyeOff : mdiEye"
                           @click:append="showPassword = !showPassword"
                        ></v-text-field>
                     </v-col>
                  </v-row>
               </v-container>
            </v-card-text>
            <v-card-actions>
               <v-spacer></v-spacer>
               <v-btn variant="text" @click="dialog = false"> Cancel </v-btn>
               <v-btn variant="text" type="submit"> Save </v-btn>
            </v-card-actions>
         </v-form>
      </v-card>
   </v-dialog>
</template>
<script setup lang="ts">
import { onBeforeUpdate } from "@vue/runtime-core";
import { defineProps, ref } from "vue";
import { mdiPlusThick, mdiEye, mdiEyeOff } from "@mdi/js";
import { SubmitEventPromise } from "vuetify/lib/framework.mjs";
import { isRequired } from "../form";
const props = defineProps({
   ssid: {
      type: String,
      default: "",
   },
   password: {
      type: String,
      default: "",
   },
   buttonIcon: {
      type: String,
      default: mdiPlusThick,
   },
   persistent: {
      type: Boolean,
      default: false,
   },
});

const emit = defineEmits<{
   (e: "cancel"): void;
   (e: "save", credential: { ssid: string; password: string }): void;
}>();
const dialog = ref<boolean>(false);
const ssid = ref<string>("");
const password = ref<string>("");
const showPassword = ref(false);

async function submit($event: SubmitEventPromise) {
   const validationResults = await $event;
   if (validationResults.valid) {
      dialog.value = false;
      emit("save", {
         ssid: ssid.value,
         password: password.value,
      });
   }
}
onBeforeUpdate(() => {
   // reset the values of ssid and password when the dialog is closed
   ssid.value = props.ssid;
   password.value = props.password;
   showPassword.value = false;
});
</script>
