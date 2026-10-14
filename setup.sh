#!/bin/bash

# Function to set up MPICH environment
function setup_mpich {
    echo "Setting up MPICH environment..."
    export MPICH_HOME=/usr/local/mpich3
    export PATH=$MPICH_HOME/bin:$PATH
}

# Function to generate RSA keys for passwordless SSH
function generate_rsa_keys {
    echo "Generating RSA keys for passwordless SSH..."
    ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
}

# Function to set up authorized_keys
function setup_authorized_keys {
    echo "Setting up authorized_keys..."
    cp ~/.ssh/id_rsa.pub ~/.ssh/authorized_keys
}

# Function to set permissions
function set_permissions {
    echo "Setting permissions..."
    chmod 644 ~/.ssh/authorized_keys
    chmod 600 ~/.ssh/id_rsa
    chmod 644 ~/.ssh/id_rsa.pub
    chmod 644 ~/.ssh/known_hosts
}

# Function to create askdate.csh script
function create_askdate_script {
    echo "Creating askdate.csh script..."
    cat << 'EOF' > ~/.ssh/askdate.csh
#!/bin/csh
foreach host (linux01 linux02 linux03 linux04 linux05 linux06 linux07 linux08 linux09 linux10 linux11 linux12 linux13 linux14 linux15 linux16 linux17 linux18 linux19 linux20 linux21 linux22 linux23 linux24 linux25 linux26 linux27 linux28 linux29 linux30)
    echo $host
    ssh $host date
end
EOF
    chmod 744 ~/.ssh/askdate.csh
}

# Function to compile the MPI program
function compile_mpi_program {
    echo "Compiling the MPI program..."
    make all
}

# Function to run the MPI program
function run_mpi_program {
    echo "Running the MPI program..."
    mpiexec -n 4 ./bin/game_of_life 1000 64
}

# Main function to run all steps
function main {
    setup_mpich
    generate_rsa_keys
    setup_authorized_keys
    set_permissions
    create_askdate_script
    compile_mpi_program
    run_mpi_program
    echo "Setup and run completed."
}

# Execute the main function
main