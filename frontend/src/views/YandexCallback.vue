<template>
  <div class="flex items-center justify-center h-screen">
    <div v-if="error" class="text-red-600 font-bold">
      Ошибка: {{ error }}
    </div>

    <div v-else-if="loading">
      Авторизация...
    </div>

    <div v-else-if="user">
      <div class="p-6 bg-white rounded-xl shadow-md w-96">
        <h2 class="text-xl font-bold mb-4">Добро пожаловать, {{ user.display_name || user.real_name }}</h2>
        <ul class="space-y-2">
          <li><b>Имя:</b> {{ user.first_name }}</li>
          <li><b>Фамилия:</b> {{ user.last_name }}</li>
          <li><b>Логин:</b> {{ user.login }}</li>
          <li><b>ID:</b> {{ user.id }}</li>
          <li><b>Пол:</b> {{ user.sex }}</li>
          <li><b>Client ID:</b> {{ user.client_id }}</li>
          <li><b>PSUID:</b> {{ user.psuid }}</li>
        </ul>
      </div>
    </div>
  </div>
</template>

<script setup>
import { onMounted, ref } from "vue"
import { useRoute } from "vue-router"
import axios from "axios"

const route = useRoute()
const loading = ref(true)
const error = ref(null)
const user = ref(null)

onMounted(async () => {
  const code = route.query.code
  if (!code) {
    error.value = "Не передан параметр code (пользователь не авторизован)"
    loading.value = false
    return
  }

  try {
    const { data } = await axios.post("http://localhost:8080/v1/oauth/callback", { code })
    user.value = data
    loading.value = false
  } catch (e) {
    console.error(e)
    error.value = "Ошибка при обмене кода или получении данных пользователя"
    loading.value = false
  }
})
</script>
