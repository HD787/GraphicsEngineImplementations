function transform(){
    let tx = 0.0, ty = 0.0, tz = 0.0, rx = 0.0, ry = 0.0, rz =0.0;
   
    if (keyStates['KeyW']){
        ty += 0.05;
    }
    if (keyStates['KeyA']){
        tx -= 0.05;
    }
    if (keyStates['KeyS']){
        ty -= 0.05;
    }
    if (keyStates['KeyD']){
        tx += 0.05;
    }
    if (keyStates['Space']){
        tz += 0.05;
    }
    if (keyStates['ShiftLeft']){
        tz -= 0.05;
    }
    if (keyStates['KeyE']){
        ry += 1.0;
    }
    if (keyStates['KeyF']){
        ry -= 1.0;
    }
    if (keyStates['KeyR']){
        rx += 1.0;
    }
    if (keyStates['KeyG']){
        rx -= 1.0;
    }
    if (keyStates['KeyT']){
        rz += 1.0;
    }
    if (keyStates['KeyH']){
        rz -= 1.0;
    }
    renderPass(wcPtr, tx, ty, tz, rx, ry, rz);
    ctx.putImageData(imageData, 0, 0);
}