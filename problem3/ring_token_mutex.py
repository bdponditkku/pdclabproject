import threading
import queue
import time
import random

NUM_PROCESSES = 3
printer_lock = threading.Lock()

class Process(threading.Thread):
    def __init__(self, pid, token_queues):
        super().__init__()
        self.pid = pid
        self.token_queues = token_queues
        self.request_print = False

    def request_to_print(self):
        self.request_print = True

    def run(self):
        while True:
            token = self.token_queues[self.pid].get()  # Wait for token
            print(f"P{self.pid+1} received token.")

            if self.request_print:
                with printer_lock:
                    print(f"*** P{self.pid+1} is printing ***")
                    time.sleep(1)  # Simulate printing
                    print(f"*** P{self.pid+1} finished printing ***")
                    self.request_print = False

            # Pass the token to the next process in ring
            next_pid = (self.pid + 1) % NUM_PROCESSES
            print(f"P{self.pid+1} passing token to P{next_pid+1}")
            time.sleep(1)
            self.token_queues[next_pid].put("TOKEN")

# Create token queues for each process
token_queues = [queue.Queue() for _ in range(NUM_PROCESSES)]

# Create and start the processes
processes = [Process(pid=i, token_queues=token_queues) for i in range(NUM_PROCESSES)]
for p in processes:
    p.start()

# Initially give token to P1
token_queues[0].put("TOKEN")

# Simulate print requests
def generate_requests():
    while True:
        time.sleep(random.randint(2, 4))
        pid = random.randint(0, NUM_PROCESSES - 1)
        processes[pid].request_to_print()
        print(f"P{pid+1} has requested to print.")

# Run request generator in a separate thread
request_thread = threading.Thread(target=generate_requests)
request_thread.daemon = True
request_thread.start()
