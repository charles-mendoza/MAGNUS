# MAGNUS
MAGNUS - Multipurpose Adaptable Graphical Nimble User Interface.

A C++ GUI Library for DirectX9/OpenGL.

![Preview](https://user-images.githubusercontent.com/28957075/68360673-f27ab080-015b-11ea-9b79-c0ea4668e151.png)

![Preview](https://user-images.githubusercontent.com/28957075/68360692-01616300-015c-11ea-96a6-9242ea9ec916.png)

# Integration
Here's some pseudo code to get started:
```

// Somewhere in your code base while the application is starting up...

// Create the renderer.
Magnus::Rendering::DirectX9* pRenderer = new Magnus::Rendering::DirectX9(g_pD3DDevice);

// Create the skin.
Magnus::Skin* pSkin = new Magnus::Skin(pRenderer);
pSkin->Load("DefaultSkin.png");

// Create the canvas.
Magnus::Controls::Canvas* pCanvas = new Magnus::Controls::Canvas(pSkin);

// Create the input system.
Magnus::Input::Windows Input;
Input.Init(pCanvas);

...

YourGameLoop()
{
    if (PeekMessage(...))
    {
        // Process events.
        Input.ProcessMessage(msg);
    }

    ...

    // Draw the canvas.
    pCanvas->RenderCanvas();
}

// Make sure to clean up the pointers.

...

```
In addition, to add a control:

```

button = new Magnus::Controls::LabeledButton(canvas);
button->GetButton()->SetText("Save");

```
