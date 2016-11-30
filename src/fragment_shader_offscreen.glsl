uniform sampler2D screenFrameBuffer;
varying vec2 vTexCoord;

void main() {
    gl_FragColor = texture2D(screenFrameBuffer, vTexCoord);
}

