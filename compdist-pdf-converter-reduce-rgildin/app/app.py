from flask import Flask, render_template, request, send_file
import os,logging,subprocess

# Configurando o logging
logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s - %(levelname)s - %(message)s',
                    filename='app.log',
                    filemode='a')
logger = logging.getLogger(__name__)

app = Flask(__name__)

def pdf_to_text(pdf_path):
    
    # Get the filename and extension of the PDF file
    file_name, file_ext = os.path.splitext(pdf_path)

    # Create the output file path with the same name as the PDF file
    output_path = file_name + '.txt'

    # Call the pdftotext command to extract text from the PDF file
    subprocess.call(['pdftotext', pdf_path, output_path])

    # Return the path to the output text file
    return output_path

def reduce_pdf_resolution(pdf_path, dpi):
    
    # Get the filename and extension of the PDF file
    file_name, file_ext = os.path.splitext(pdf_path)

    # Create the output file path with the same name as the PDF file
    output_path = file_name + f'_{dpi}dpi{file_ext}'

    # Call the gs command to reduce the resolution of the PDF file
    subprocess.call(['gs', '-q', '-dNOPAUSE', '-dBATCH', f'-sDEVICE=pdfwrite', f'-dPDFSETTINGS=/screen', f'-dCompatibilityLevel=1.4', f'-dProcessColorModel=/DeviceGray', f'-dColorConversionStrategy=/Gray', f'-dAutoRotatePages=/None', f'-dDownsampleColorImages=true', f'-dColorImageDownsampleType=/Bicubic', f'-dColorImageResolution={dpi}', f'-dGrayImageDownsampleType=/Bicubic', f'-dGrayImageResolution={dpi}', f'-dMonoImageDownsampleType=/Bicubic', f'-dMonoImageResolution={dpi}', f'-sOutputFile={output_path}', pdf_path])

    # Return the path to the output PDF file
    return output_path

@app.route('/')
def index():
    return render_template('home.html')

@app.route('/convert-to-text')
def convert_to_text():
    return render_template('convert.html')

@app.route('/reduce-resolution')
def reduce_resolution():
    return render_template('reduce.html')

@app.route('/convert', methods=['POST'])
def convert():
    # Get the uploaded file
    file = request.files['file']

    # Save the file to disk
    file_name, file_ext = os.path.splitext(file.filename)
    file_path = os.path.join('uploads', file.filename)
    file.save(file_path)

    # Convert the PDF file to text
    text_path = pdf_to_text(file_path)

    logger.info(f'Arquivo pdf "{file_name}.pdf" convertido em texto "{file_name}.txt"')
    return send_file(text_path, as_attachment=True, download_name=os.path.basename(text_path))

@app.route('/reduce', methods=['POST'])
def reduce():
    # Get the uploaded file
    file = request.files['file']

    # Save the file to disk
    file_name, file_ext = os.path.splitext(file.filename)
    file_path = os.path.join('uploads', file.filename)
    file.save(file_path)

    # Get the resolution to reduce to
    dpi = int(request.form['resolution'])

    # Reduce the resolution of the PDF file
    reduced_path = reduce_pdf_resolution(file_path, dpi)

    # Return the reduced PDF file to the user
    logger.info(f'Arquivo pdf "{file_name}.pdf" foi reduzido para {dpi} dpi em "{reduced_path.split("/")[1]}"')
    return send_file(reduced_path, as_attachment=True, download_name=os.path.basename(reduced_path))

if __name__ == '__main__':  
    app.run(debug=True)