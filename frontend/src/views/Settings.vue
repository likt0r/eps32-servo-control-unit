<template>
   <v-row no-gutters justify="center" cols="12">
      <v-col xs="12" md="10" lg="8" xl="6" class="pa-2">
         <v-card>
            <v-card-title> WiFi-Credentials </v-card-title>
            <v-card-text>
               <v-list>
                  <transition-group>
                     <draggable
                        class="list-group"
                        item-key="name"
                        key="dragggable"
                        :list="credentials"
                        v-bind="dragOptions"
                        @start="drag = true"
                        @end="drag = false"
                     >
                        <template #item="{ element: cred, index }">
                           <v-list-item
                              :key="cred.ssid"
                              :title="cred.ssid"
                              :subtitle="
                                 showPasswords ? cred.password : '********'
                              "
                              background="surface"
                           >
                              <template v-slot:prepend>
                                 <v-icon
                                    class="list-group-item"
                                    :icon="mdiDragHorizontalVariant"
                                 ></v-icon>
                              </template>
                              <template v-slot:append>
                                 <SsidCredentialsDialog
                                    :buttonIcon="mdiPencil"
                                    :ssid="cred.ssid"
                                    :password="cred.password"
                                    @save="
                                       (event) => {
                                          onSave(
                                             event.ssid,
                                             event.password,
                                             index
                                          );
                                       }
                                    "
                                 />
                                 <v-btn
                                    :icon="mdiDelete"
                                    variant="text"
                                    @click="() => onDelete(index)"
                                 ></v-btn>
                              </template>
                           </v-list-item>
                        </template>
                     </draggable>
                  </transition-group>
                  <v-list-item background="surface">
                     <template v-slot:append>
                        <v-btn
                           :icon="showPasswords ? mdiEye : mdiEyeOff"
                           variant="text"
                           @click="() => (showPasswords = !showPasswords)"
                        ></v-btn>
                        <SsidCredentialsDialog
                           @save="
                              (event) => {
                                 onSave(event.ssid, event.password, undefined);
                              }
                           "
                        />
                     </template>
                  </v-list-item>
               </v-list>
            </v-card-text>
         </v-card>
      </v-col>
   </v-row>
</template>

<script lang="ts">
import { defineComponent, ComponentPublicInstance, ref } from "vue";
import apiService from "@/ApiService";
import { WifiCredential } from "@/ApiService";
import draggable from "vuedraggable";
import { nanoid } from "nanoid";
import SsidCredentialsDialog from "@/components/SsidCredentialsDialog.vue";

interface IInstance extends ComponentPublicInstance {
   setViewData(data: WifiCredential[]): void;
}

export default defineComponent({
   async beforeRouteEnter(to, from, next) {
      try {
         const wifiCredentials = (await apiService.getWifiCredentials()).data;

         next((vm) => {
            const instance = vm as IInstance;
            instance.setViewData(wifiCredentials);
         });
      } catch (error) {
         console.log("error", error);
         next(false);
      }
   },
});
</script>

<script setup lang="ts">
import {
   mdiDragHorizontalVariant,
   mdiDelete,
   mdiPencil,
   mdiEye,
   mdiEyeOff,
} from "@mdi/js";

const credentials = ref<WifiCredential[]>([]);

function setViewData(data: WifiCredential[]) {
   credentials.value = data;
}

// const Draggable = draggable.default;

const dragOptions = ref({
   animation: 200,
   group: "description",
   disabled: false,
   ghostClass: "ghost",
});
const drag = ref(false);
const showPasswords = ref(false);

const onSave = (ssid: string, password: string, index: number | undefined) => {
   if (index) {
      credentials.value[index].ssid = ssid;
      credentials.value[index].password = password;
   } else {
      credentials.value.push({ ssid, password });
   }
};

const onDelete = (index: number) => {
   credentials.value.splice(index, 1);
};
</script>

<style>
.ghost {
   opacity: 0.5;
   background: #c8ebfb;
}

.list-group-item {
   cursor: move;
}
</style>
