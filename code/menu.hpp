#ifndef MENU_HPP
#define MENU_HPP

/**
 * @brief 显示游戏菜单，并设置玩家类型
 * @param blackPlayerType 黑棋玩家类型，true为AI，false为人类
 * @param whitePlayerType 白棋玩家类型，true为AI，false为人类
 */
void displayMenu(bool &blackPlayerType, bool &whitePlayerType);

/**
 * @brief 暂停等待用户按回车键
 */
void pauseBeforeUpdate();

#endif
