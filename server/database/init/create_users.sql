-- Create user
CREATE USER 'sensor_user'@'%' IDENTIFIED BY 'sensor_chimchimlin';

-- Set global permissions
GRANT ALL PRIVILEGES ON sensor_DB.* TO 'sensor_user'@'%';

-- Refresh permissions
FLUSH PRIVILEGES;
