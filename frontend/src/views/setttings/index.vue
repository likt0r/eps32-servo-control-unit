<script lang="ts" setup>
import { onMounted, ref } from "vue";
import draggable from "vuedraggable";
import {
   mdiDragHorizontalVariant,
   mdiDelete,
   mdiPencil,
   mdiEye,
   mdiEyeOff,
} from "@mdi/js";
import SsidCredentialsDialog from "@/components/SsidCredentialsDialog.vue";
import ViewWrapper from "@/components/ViewWrapper.vue";
import apiService from "@/ApiService";
import { WifiCredential } from "@/ApiService";
import useAppStore from "@/store/app";
const { publishAlert } = useAppStore();

const credentials = ref<WifiCredential[]>([]);
const loadingCredentials = ref(false);
const credentialsChanged = ref(false);

// const Draggable = draggable.default;

const dragOptions = ref({
   animation: 200,
   group: "description",
   disabled: false,
   ghostClass: "ghost",
});
const drag = ref(false);
const showPasswords = ref(false);

async function onChange(
   ssid: string,
   password: string,
   index: number | undefined
) {
   if (index) {
      credentials.value[index].ssid = ssid;
      credentials.value[index].password = password;
   } else {
      credentials.value.push({ ssid, password });
   }
   credentialsChanged.value = true;
}
async function onSave() {
   loadingCredentials.value = true;
   await apiService.setWifiCredentials(credentials.value);
   loadingCredentials.value = false;
   credentialsChanged.value = false;
}

const onDelete = (index: number) => {
   credentials.value.splice(index, 1);
   credentialsChanged.value = true;
};
onMounted(async () => {
   loadingCredentials.value = true;
   try {
      loadingCredentials.value = true;
      const response = await apiService.getWifiCredentials();
      credentials.value = response.data;
   } catch (error: any) {
      console.log("error", error);
      publishAlert(
         "error",
         "Could not load WiFi-Credentials",
         JSON.stringify(error)
      );
   }
   loadingCredentials.value = false;
});
</script>
<template>
   <ViewWrapper>
      <v-row no-gutters justify="center" cols="12">
         <v-col xs="12" md="10" lg="8" xl="6" class="pa-2">
            <v-card>
               <template v-slot:loader="{ isActive }">
                  <v-progress-linear
                     :active="loadingCredentials"
                     color="primary"
                     height="8"
                     indeterminate
                  ></v-progress-linear>
               </template>
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
                           @end="
                              {
                                 drag = false;
                                 credentialsChanged = true;
                              }
                           "
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
                                             onChange(
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
                                    onChange(
                                       event.ssid,
                                       event.password,
                                       undefined
                                    );
                                 }
                              "
                           />
                        </template>
                     </v-list-item>
                  </v-list>
               </v-card-text>
               <v-card-actions>
                  <v-spacer></v-spacer>
                  <v-btn
                     @click="onSave"
                     color="primary"
                     :disabled="!credentialsChanged || loadingCredentials"
                  >
                     Save
                  </v-btn>
               </v-card-actions>
            </v-card>
         </v-col>
      </v-row>
   </ViewWrapper>
</template>

<style>
.ghost {
   opacity: 0.5;
   background: #c8ebfb;
}

.list-group-item {
   cursor: move;
}
</style>
