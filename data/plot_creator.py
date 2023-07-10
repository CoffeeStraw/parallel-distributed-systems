import pandas as pd
import matplotlib.pyplot as plt

no_io = ""
if input("No_io? ") == "y":
    no_io = "_no_io"

# Load the data from the CSV files
sequential_data = pd.read_csv('sequential.csv')
thread_data = pd.read_csv('thread.csv')
fastflow_data = pd.read_csv('fastflow.csv')
io_data = pd.read_csv('io.csv')

# remove .txt from filePath
sequential_data['filePath'] = sequential_data['filePath'].str.replace('.txt', '')
thread_data['filePath'] = thread_data['filePath'].str.replace('.txt', '')
fastflow_data['filePath'] = fastflow_data['filePath'].str.replace('.txt', '')
io_data['filePath'] = io_data['filePath'].str.replace('.txt', '')

if no_io:
    # Subtract the time spent on IO from the sequential data
    sequential_data['average'] = sequential_data['average'] - io_data['average']

    # Subtract the time spent on IO from the thread data and the fastflow data
    for filePath in thread_data['filePath'].unique():
        io_average = io_data.loc[io_data['filePath'] == filePath, 'average'].iloc[0]
        thread_data.loc[thread_data['filePath'] == filePath, 'average'] = thread_data.loc[thread_data['filePath'] == filePath, 'average'] - io_average
        fastflow_data.loc[fastflow_data['filePath'] == filePath, 'average'] = fastflow_data.loc[fastflow_data['filePath'] == filePath, 'average'] - io_average


# Create a new column on thread_data and fastflow_data to store the speedup, the scalability
for filePath in sequential_data['filePath'].unique():
    sequential_average = sequential_data.loc[sequential_data['filePath'] == filePath, 'average'].iloc[0]
    thread_average_1 = thread_data.loc[(thread_data['filePath'] == filePath) & (thread_data['nWorkers'] == 1), 'average'].iloc[0]
    fastflow_average_1 = fastflow_data.loc[(fastflow_data['filePath'] == filePath) & (fastflow_data['nWorkers'] == 1), 'average'].iloc[0]

    thread_data.loc[thread_data['filePath'] == filePath, 'speedup'] = sequential_average / thread_data.loc[thread_data['filePath'] == filePath, 'average']
    fastflow_data.loc[fastflow_data['filePath'] == filePath, 'speedup'] = sequential_average / fastflow_data.loc[fastflow_data['filePath'] == filePath, 'average']

    thread_data.loc[thread_data['filePath'] == filePath, 'scalability'] = thread_average_1 / thread_data.loc[thread_data['filePath'] == filePath, 'average']
    fastflow_data.loc[fastflow_data['filePath'] == filePath, 'scalability'] = fastflow_average_1 / fastflow_data.loc[fastflow_data['filePath'] == filePath, 'average']

# Convert average from us to ms
sequential_data['average'] = sequential_data['average'] / 1000
thread_data['average'] = thread_data['average'] / 1000
fastflow_data['average'] = fastflow_data['average'] / 1000

markers_thread = ['.', 'v', 'P']
colors_thread = ['darkseagreen', 'yellowgreen', 'green']
markers_fastflow = ['d', '*', 'x']
colors_fastflow = ['rosybrown', 'lightcoral', 'brown']

for i, filePath in enumerate(sequential_data['filePath'].unique()):
    # Plot the average, one curve in the same plot for each filePath
    # using colors
    fig, ax = plt.subplots(figsize=(10,6))

    # Plot the average for sequential (since nWorkers is always 1, we can use the same data for all the files)
    x = [i for i in range(0, 65)]
    y = [sequential_data.loc[sequential_data['filePath'] == filePath, 'average'].iloc[0] for i in range(0, 65)]
    ax.plot(x, y, color='blue', label=f'Sequential ({filePath})')

    # Plot the average for thread
    ax.plot(thread_data.loc[thread_data['filePath'] == filePath, 'nWorkers'], thread_data.loc[thread_data['filePath'] == filePath, 'average'], marker=markers_thread[i], color=colors_thread[i], label=f'Thread ({filePath})')

    # Plot the average for fastflow
    ax.plot(fastflow_data.loc[fastflow_data['filePath'] == filePath, 'nWorkers'], fastflow_data.loc[fastflow_data['filePath'] == filePath, 'average'], marker=markers_fastflow[i], color=colors_fastflow[i], label=f'FastFlow ({filePath})')

    # Set x-axis ticks and limits
    ax.set_xticks([i for i in range(0, 65, 2)])

    # Add gridlines
    ax.grid(True, which='both', linestyle='--')

    # Add legend on top
    ax.legend(loc='upper center', bbox_to_anchor=(0.5, 1.14), ncol=3)

    # Set plot title and axis labels
    plt.xlabel('Number of workers')
    plt.ylabel('Average completion time (ms)')

    # Save the plot
    plt.savefig(f'imgs/average_{filePath}{no_io}.png')

    # Clear the plot
    plt.clf()

# Plot the speedup, one curve in the same plot for each filePath
# using colors
fig, ax = plt.subplots(figsize=(10,6))

for i, filePath in enumerate(sequential_data['filePath'].unique()):
    # Plot the speedup for thread
    ax.plot(thread_data.loc[thread_data['filePath'] == filePath, 'nWorkers'], thread_data.loc[thread_data['filePath'] == filePath, 'speedup'], marker=markers_thread[i], color=colors_thread[i], label=f'Thread ({filePath})')

    # Plot the speedup for fastflow
    ax.plot(fastflow_data.loc[fastflow_data['filePath'] == filePath, 'nWorkers'], fastflow_data.loc[fastflow_data['filePath'] == filePath, 'speedup'], marker=markers_fastflow[i], color=colors_fastflow[i], label=f'FastFlow ({filePath})')

# Set x-axis ticks and limits
ax.set_xticks([i for i in range(0, 65, 2)])

# Add gridlines
ax.grid(True, which='both', linestyle='--')

# Add legend on top
ax.legend(loc='upper center', bbox_to_anchor=(0.5, 1.14), ncol=3)

# Set plot title and axis labels
plt.xlabel('Number of workers')
plt.ylabel('Average speedup')

# Save the plot
plt.savefig(f'imgs/speedup{no_io}.png')

# Clear the plot
plt.clf()

# Plot the scalability, one curve in the same plot for each filePath
fig, ax = plt.subplots(figsize=(10,6))

for i, filePath in enumerate(sequential_data['filePath'].unique()):
    filepath = filePath.replace('.txt', '')

    # Plot the scalability for thread
    ax.plot(thread_data.loc[thread_data['filePath'] == filePath, 'nWorkers'], thread_data.loc[thread_data['filePath'] == filePath, 'scalability'], marker=markers_thread[i], color=colors_thread[i], label=f'Thread ({filePath})')

    # Plot the scalability for fastflow
    ax.plot(fastflow_data.loc[fastflow_data['filePath'] == filePath, 'nWorkers'], fastflow_data.loc[fastflow_data['filePath'] == filePath, 'scalability'], marker=markers_fastflow[i], color=colors_fastflow[i], label=f'FastFlow ({filePath})')

# Set x-axis ticks and limits
ax.set_xticks([i for i in range(0, 65, 2)])

# Add gridlines
ax.grid(True, which='both', linestyle='--')

# Add legend on top
ax.legend(loc='upper center', bbox_to_anchor=(0.5, 1.14), ncol=3)

# Set plot title and axis labels
plt.xlabel('Number of workers')
plt.ylabel('Average scalability')

# Save the plot
plt.savefig(f'imgs/scalability{no_io}.png')

# Clear the plot
plt.clf()