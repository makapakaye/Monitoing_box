/**
 * Server config
 */
export type Config = {
    apiConfig: ApiConfig;
    ipBlocker: IPBlockerConfig
}

/**
 * api 設置及開關
 * @param {string} host - host (應設置為 localhost)
 * @param {number} port - port
 * @param {Object} enableModule - 是否啟用子模塊
 */
export type ApiConfig = {
    host: string;
    port: number;
    enableModule: {}
}

/**
 * IPBlocker config
 * @param {number} retryLimit - 重試次數
 * @param {number} unlockTimeoutDuration - 封鎖時間(ms)
 * @param {number} cleanupInterval - 定時清理器時間(ms)
 */
export type IPBlockerConfig = {
    retryLimit: number;
    unlockTimeoutDuration: number;
    cleanupInterval: number;
}




/**
 * Enviornment config
 */
export interface EnvConfig {
    salt: string;
    dbConfig: DBConfig;
    smtpConfig: SmtpConfig;
}

/**
 * Database config
 */
export interface DBConfig {
    host: string;
    port: number;
    database: string;
    user: string;
    password: string;
}

/**
 * SMTP account config
 */
export interface SmtpConfig {
    user: string;
    password: string;
}