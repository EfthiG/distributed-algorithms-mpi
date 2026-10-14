import os
import numpy as np
import matplotlib.pyplot as plt

def read_multiply_data(lines):
    data = {}

    for line in lines[1:]:
        parts = line.split()
        if len(parts) != 3:
            continue

        nprocs = int(parts[0])
        size = int(parts[1])
        time = float(parts[2])

        key = (nprocs, size)
        if key not in data:
            data[key] = []
        data[key].append(time)

    # Calculate the average time for each key
    for key in data:
        data[key] = np.mean(data[key])

    return data


def read_game_of_life_data(lines):
    data = {}

    for line in lines[1:]:
        parts = line.split()
        if len(parts) != 4:
            continue

        nprocs = int(parts[0])
        generations = int(parts[1])
        size = int(parts[2])
        time = float(parts[3])

        key = (nprocs, generations, size)
        if key not in data:
            data[key] = []
        data[key].append(time)

    # Calculate the average time for each key
    for key in data:
        data[key] = np.mean(data[key])

    return data


def read_data(text_file):
    data = []
    with open(text_file, 'r') as file:
        lines = file.readlines()

    if "multiply" in text_file:
        return read_multiply_data(lines)
    elif "game_of_life" in text_file:
        return read_game_of_life_data(lines)
    else:
        return None


def plot_multiply_data(data):
    sizes = [60, 240, 2400, 6000]
    nprocs = [1, 2, 3, 4]
    labels = [str(t) for t in nprocs]

    for size in sizes:
        times = []
        for p in nprocs:
            key = (p, size)
            times.append(data.get(key, 0))

        plt.figure()
        plt.bar(labels, times, color=['#003f5c', '#43aa8b', '#2f4b7c', '#2f9e44'])
        # plt.plot(labels, times, marker='o')
        plt.xlabel('Processes')
        plt.ylabel('Time (s)')
        plt.title(f'Execution Time for Size {size}')
        plt.savefig(f"results/multiply_{size}.png")



def plot_game_of_life_data(data):
    sizes = [64, 1024, 2048]
    nprocs = [1, 2, 3, 4]
    labels = [str(t) for t in nprocs]

    for size in sizes:
        times = []
        for p in nprocs:
            key = (p, 1000, size)
            times.append(data.get(key, 0))

        plt.figure()
        plt.bar(labels, times, color=['#003f5c', '#43aa8b', '#2f4b7c', '#2f9e44'])
        # plt.plot(labels, times, marker='o')
        plt.xlabel('Processes')
        plt.ylabel('Time (s)')
        plt.title(f'Execution Time for Size {size}x{size}')
        plt.savefig(f"results/game_of_life_{size}.png")


def plot_data(data, text_file):
    if "multiply" in text_file:
        plot_multiply_data(data)
    elif "game_of_life" in text_file:
        plot_game_of_life_data(data)
    else:
        return None


if __name__ == "__main__":
    results_folder = "results/"
    results_files = [
        'multiply.txt',
        'game_of_life.txt'
    ]

    for text_file in results_files:
        text_file = results_folder + text_file

        if not os.path.exists(text_file):
            print(f"File {text_file} does not exist")
            continue

        data = read_data(text_file)
        plot_data(data, text_file)