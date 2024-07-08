/**
 * 配置檔 (所有值須設置否則會出錯)
 * 
 * @type {import("./src/@types/Config.types.js").Config} - config
 * 
 * @param {Object} apiConfig - api 設置及開關
 * @param {string} apiConfig.host - api host
 * @param {number} apiConfig.port - api port
 * @param {Object} apiConfig.enableModule - 是否啟用子模塊
 * 
 * @param {Object} IPBlocker - IPBlocker config
 * @param {number} IPBlocker.retryLimit - 重試次數 (default: 5)
 * @param {number} IPBlocker.unlockTimeoutDuration - 封鎖時間(ms) (default: 5 minutes)
 * @param {number} IPBlocker.cleanupInterval - 定時清理器時間(ms) (default: 5 minutes)
 * 
 */
const config = {
    apiConfig: {
        host: '0.0.0.0',
        port: 4000,
        enableModule: {
        }
    },
    ipBlocker: {
        retryLimit: 5,
        unlockTimeoutDuration: 5 * 60 * 1000,
        cleanupInterval: 5 * 60 * 1000
    }
};

export { config };