# 历史贡献者声明

- **Hathing** 是 2025.8-2025.11 期间以下代码的**原始作者**：
  > 后续维护者修改/删除代码前，必须在文件内部留下原始作者署名注释，并在修改日志中注明原始作者。  
  - [PR完整列表](https://github.com/BarleyModCommittee/BarleyMod/pulls?q=is%3Apr+is%3Aclosed+author%3AHathing)
  - **核心事件系统框架（不完整统计）**  
      `pvzdll/MyClasses.cpp`  
      `pvzdll/MyEvents.hpp`
    
    - **植物**  
      `pvzdll/MyPlant/`（植物框架）  
      `pvzdll/PlantEvents.cpp`（植物行为逻辑）  
      *关键特性：（不完整统计）*  
      - 植物多连发/多投/大招技能系统（机枪/三线/玉米等）  
      - 植物亡语触发机制（土豆雷/爆坚等）  
      - 植物体型/颜色/状态动态修改系统  
      - 植物特殊技能
    
    - **子弹**  
      `pvzdll/MyProjectile/`（子弹框架）  
      `pvzdll/ProjectileEvents.cpp`（子弹行为逻辑）  
      *关键特性：（不完整统计）*  
      - 穿透子弹运动处理（裂荚小子弹）  
      - 子弹反飞/反弹/弹跳机制  
      - 三线子弹正弦运动轨迹  
      - 元素伤害叠加系统（寒意/燃烧/爆炸）
    
    - **僵尸**  
      `pvzdll/MyZombie/`（僵尸框架）  
      `pvzdll/ZombieEvents.cpp`（僵尸行为逻辑）  
      *关键特性：（不完整统计）*  
      - 僵尸击退/击飞/飞跃物理系统  
      - 元素状态效果（寒意减速/燃烧伤害）  
      - 特殊僵尸行为：雪橇队/扶梯/盲盒/辣椒头  
      - 僵尸颜色标记与状态可视化
    
    - **其他**  
      `pvzdll/MyResources.cpp`（资源加载框架）  
      `pvzdll/MyBoard.cpp`（游戏状态管理）  
      `pvzdll/BoardEvents.cpp`   
      `pvzdll/CoinEvents.cpp`    
      *关键特性：（不完整统计）*  
      - 动态血条/等级UI绘制系统  
      - 金币自动收集与经验分配代码重构  
      - 无特效种植与自由种植支持

- **GitHub 统计说明**：  
  - Hathing 的原始提交历史在 [GitHub 贡献者统计](https://github.com/BarleyModCommittee/BarleyMod/graphs/contributors) 中被归于合并者 (W1D0)。
  - Hathing 在 2025.8-2025.11 期间使用的 Git 提交用户名为Lenovo。

- **Hathing 代码贡献统计** (2025.8-2025.11)
  - 总添加行数 (Additions): 8173
  - 总删除行数 (Deletions): 1555
  - 净增加行数 : 6618
