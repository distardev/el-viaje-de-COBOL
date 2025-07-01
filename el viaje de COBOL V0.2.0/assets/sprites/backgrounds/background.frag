// background.frag
uniform sampler2D texture;
uniform float time;
uniform vec2 resolution;

void main()
{
    // Coordenadas normalizadas [0,1]
    vec2 uv = gl_FragCoord.xy / resolution;

    // Parámetros para las ondas
    float waveFreqX = 10.0;
    float waveFreqY = 15.0;
    float waveSpeedX = 0.15;
    float waveSpeedY = 0.3;
    float waveAmplitude = 0.02;

    // Efecto onda en X desplazado en función de Y y tiempo
    float waveX = sin(uv.y * waveFreqX + time * waveSpeedX) * waveAmplitude;

    // Efecto onda en Y desplazado en función de X y tiempo (otro tipo de onda)
    float waveY = cos(uv.x * waveFreqY + time * waveSpeedY) * waveAmplitude;

    // Combino las ondas para hacer el desplazamiento UV más fluido
    uv.x += waveX;
    uv.y += waveY;

    // Muestro la textura con las coordenadas modificadas
    gl_FragColor = texture2D(texture, uv);
}
