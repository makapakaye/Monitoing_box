<template>
  <div>
    <h1>Sensor Data</h1>
    <ul>
      <li v-for="(data, index) in sensorData" :key="index">
        Value: {{ data.temperature }}
      </li>
    </ul>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  data() {
    return {
      sensorData: []
    };
  },
  methods: {
    async fetchSensorData() {
      const url = '/api/sensor/sensorData';
      const params = { sensor_id: 1, limit_value: 10 };

      try {
        const response = await axios.get(url, { params });
        console.log('Fetched data:', response.data);
        this.sensorData = response.data;
      } catch (error) {
        console.error('Error fetching sensor data:', error);
      }
    }
  },
  mounted() {
    this.fetchSensorData();
  }
};
</script>

<style scoped>
main {
  background-color: #000;
}
</style>
