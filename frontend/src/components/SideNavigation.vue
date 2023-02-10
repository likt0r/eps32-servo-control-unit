<script setup lang="ts">
export interface NavPoint {
  title: string;
  icon: string;
  to: string;
}

export interface Props {
  navPoints: NavPoint[];
}

const props = withDefaults(defineProps<Props>(), {
  navPoints: () => [],
});
</script>

<template>
  <v-list dense nav>
    <v-list-item
      v-for="item in props.navPoints"
      :key="item.title"
      link
      :to="item.to"
    >
      <template v-slot:prepend>
        <v-icon :icon="item.icon"></v-icon>
      </template>


        <v-list-item-title>{{ item.title }}</v-list-item-title>

    </v-list-item>
  </v-list>
</template>
