# GLSL 偏导数详解

## 核心问题

**OpenGL GLSL中求偏导数，每个核心都是独立运行，为什么可以求偏导？**

这是一个很好的问题，它触及了GPU并行架构和导数计算的本质。

## 一、GPU的并行执行模式

### 1.1 2×2 Quad 执行单元

虽然每个片元着色器核心确实是"独立运行"的，但关键点在于：

- **片元不是单独处理的**：GPU不会单独处理单个片元，而是以 **2×2的片元块（quad）** 为单位进行调度
- **同步执行**：在同一个warp/wavefront中，相邻的4个片元会被一起处理

```
[ (x,y)     (x+1,y)   ]
[ (x,y+1)   (x+1,y+1) ]
```

### 1.2 为什么采用Quad设计？

这种设计使得：
1. 硬件可以高效地计算相邻片元之间的差值
2. 不需要额外的通信开销
3. 特别适合基于屏幕空间的效果（法线计算、边缘检测等）

## 二、硬件级别的导数计算

### 2.1 偏导数函数

GLSL提供了以下偏导数函数：

- `dFdx(p)` - 计算变量p在水平方向（x轴）的偏导数
- `dFdy(p)` - 计算变量p在垂直方向（y轴）的偏导数
- `fwidth(p)` - 等价于 `abs(dFdx(p)) + abs(dFdy(p))`

### 2.2 计算原理

在2×2的quad内：
- **dFdx()** = 右侧片元值 - 左侧片元值
- **dFdy()** = 下方片元值 - 上方片元值

这些计算在硬件层面完成，利用了片元在quad中的空间关系。

### 2.3 执行流程

```glsl
// GLSL示例
void main() {
    float height = texture(heightMap, texCoord).r;
    float slopeX = dFdx(height);  // 水平方向变化率
    float slopeY = dFdy(height);  // 垂直方向变化率
    // ... 用于法线计算等
}
```

硬件执行步骤：
1. GPU调度一个2×2的quad同时执行
2. 四个片元并行计算各自的变量值
3. 硬件比较相邻片元的计算结果
4. 为每个片元提供其与邻居的差值作为导数

## 三、实际应用场景

### 3.1 法线贴图计算

```glsl
// 使用导数计算法线（高效方法）
vec3 calculateNormal() {
    float height = texture(heightMap, texCoord).r;
    
    // 使用导数：只需要1次纹理采样
    vec3 normal = normalize(vec3(-dFdx(height), -dFdy(height), 1.0));
    return normal;
}

// 传统方法对比：需要3次纹理采样
vec3 calculateNormalTraditional() {
    float heightCenter = texture(heightMap, texCoord).r;
    float heightRight = texture(heightMap, texCoord + vec2(texelSize.x, 0.0)).r;
    float heightUp = texture(heightMap, texCoord + vec2(0.0, texelSize.y)).r;
    
    vec3 normal = normalize(vec3(heightCenter - heightRight, heightCenter - heightUp, 1.0));
    return normal;
}
```

### 3.2 Mipmap级别选择

```glsl
// GPU自动使用dFdx/dFdy来选择合适的mipmap级别
vec4 color = texture(myTexture, texCoord);

// 手动计算mipmap级别
float mipLevel = 0.5 * log2(max(
    dot(dFdx(texCoord), dFdx(texCoord)),
    dot(dFdy(texCoord), dFdy(texCoord))
));
```

## 四、限制和注意事项

### 4.1 动态分支问题

```glsl
// ⚠️ 危险：在动态分支中使用偏导数可能得到未定义结果
if (someCondition) {
    float derivative = dFdx(value);  // 可能不可靠！
}
```

**原因**：quad中部分片元可能跳过计算，导致无法有效比较相邻片元的值。

### 4.2 Quad边界限制

**关键限制**：导数只在2×2的quad内部计算，**不会跨quad计算**

对于4×4的片元区域，GPU会将其分成4个独立的2×2 quad：

```
Quad A      Quad B
[0,0][0,1]  [0,2][0,3]
[1,0][1,1]  [1,2][1,3]

Quad C      Quad D  
[2,0][2,1]  [2,2][2,3]
[3,0][3,1]  [3,2][3,3]
```

- 每个quad内部独立计算导数
- 在quad边界处（例如[0,1]和[0,2]之间），导数可能出现不连续
- 这可能导致可见的2×2块状图案

### 4.3 为什么不跨Quad计算？

**性能考量**：
- 保持quad的独立性简化了硬件设计
- 跨quad通信会显著增加复杂性和延迟
- 对于大多数实时图形应用，quad内导数已经足够

## 五、跨Quad导数的解决方案

### 5.1 手动计算导数

```glsl
// 使用显式的相邻像素采样（更准确但更慢）
float manualDfdx(sampler2D tex, vec2 texCoord) {
    float right = textureOffset(tex, texCoord, ivec2(1, 0)).r;
    float left = textureOffset(tex, texCoord, ivec2(-1, 0)).r;
    return (right - left) * 0.5;  // 中心差分
}

float manualDfdy(sampler2D tex, vec2 texCoord) {
    float up = textureOffset(tex, texCoord, ivec2(0, 1)).r;
    float down = textureOffset(tex, texCoord, ivec2(0, -1)).r;
    return (up - down) * 0.5;
}
```

### 5.2 使用卷积核

```glsl
// 使用3×3 Sobel算子
float sobelDx(sampler2D tex, vec2 texCoord) {
    float result = 
        textureOffset(tex, texCoord, ivec2(1, 1)).r +
        2.0 * textureOffset(tex, texCoord, ivec2(1, 0)).r +
        textureOffset(tex, texCoord, ivec2(1, -1)).r -
        textureOffset(tex, texCoord, ivec2(-1, 1)).r -
        2.0 * textureOffset(tex, texCoord, ivec2(-1, 0)).r -
        textureOffset(tex, texCoord, ivec2(-1, -1)).r;
    return result;
}
```

### 5.3 性能权衡

```glsl
// 根据需求选择合适的方法
if (highQualityRequired) {
    derivative = manualDfdx(tex, texCoord);  // 高质量，多次采样
} else {
    derivative = dFdx(value);  // 硬件加速，更快
}
```

## 六、总结

### 核心要点

1. **执行单元**：片元着色器以2×2的quad为单位并行执行，而非完全独立
2. **导数计算**：通过quad内相邻片元的差值在硬件层面高效计算
3. **性能优势**：几乎零开销，特别适合实时图形应用
4. **主要限制**：
   - 只在quad内有效，不跨quad计算
   - 动态分支中可能不可靠
   - quad边界可能出现不连续

### 适用场景

✅ **推荐使用内置偏导数的场景**：
- 法线贴图计算
- Mipmap级别选择
- 简单的边缘检测
- 各向异性过滤

⚠️ **需要手动计算的场景**：
- 高质量法线重建
- 跨像素的精确边缘检测
- 需要跨quad一致性的算法
- 对导数精度要求极高的应用

### 最终答案

**偏导数能够计算的根本原因**：虽然片元着色器核心在逻辑上是独立的，但GPU的调度机制确保了相邻片元（以2×2 quad为单位）在同一时刻执行相同的指令流，这使得硬件能够实时比较它们的计算结果并高效计算导数。

这种设计体现了GPU在并行计算和图形处理之间的精妙平衡！

---

## 参考资源

- OpenGL Shading Language Specification
- GPU Gems系列关于导数计算的章节
- 各GPU厂商的技术文档（NVIDIA、AMD、Intel）

