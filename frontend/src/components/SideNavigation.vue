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
    <v-list-item v-for="item in props.navPoints" :key="item.title" link>
      <v-list-item-icon>
        <v-icon>{{ item.icon }}</v-icon>
      </v-list-item-icon>

      <v-list-item-content>
        <v-list-item-title>{{ item.title }}</v-list-item-title>
      </v-list-item-content>
    </v-list-item>
  </v-list>
</template>
