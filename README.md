### Thread Safe Queue - Single Consumer

```cpp
    ThreadSafeQueueMPSC<int> queue;

    // Producer
    queue.push(3); // lock

    // SINGLE Consumer
    if(not queue.is_empty()) // atomic check
    {
        int val = queue.get_front_and_pop(); // lock
    }

```
