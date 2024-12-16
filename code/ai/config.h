// config.h
#ifndef CONFIG_H
#define CONFIG_H

struct Config {
    bool enableCache = true;      // 是否开启缓存
    int pointsLimit = 20;         // 每一层最多搜索节点数
    bool onlyInLine = false;      // 是否只搜索一条线上的点位
    int inlineCount = 4;          // 最近多少个点位能算作
    int inLineDistance = 5;       // 判断点位是否在一条线上的最大距离
};

extern Config config;

#endif // CONFIG_H
